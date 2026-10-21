#include "Display.h"

#include <cmath>

#include "core/assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/hardware/spi/SpiDma.h"
#include "core/rtos/MutexGuard.h"
#include "core/util/helpers/ByteUtils.h"
#include "core/util/color/types/Rgb565.h"
#include "core/util/color/types/Rgb888.h"
#include "core/util/color/types/HslColor.h"

#define GarboxDebugDisplay 0

namespace Garbox {

static_assert(LvglHandler::DrawBufferCount == DisplayBufferTracker::Capacity);

Display::Display(const Config& config): 
    // init members 
    mSpi(config.spi),
    mSt7789v(config.gpioRst, config.gpioDc, config.gpioCs, config.pwmBlk),
    mLvglHandler(LvglHandler::Config{
        .width = config.width,
        .height = config.height,
        .bytesPerPixel = config.bytesPerPixel,
        .partialFactor = config.partialFactor,
    }),
    mChunkSender(mSpi, config.chunkSizeBytes),
    mWidth(config.width),
    mHeight(config.height){
    // constructor body
}

LvglHandler& Display::getLvglHandler(){
    return mLvglHandler;
}

void Display::init(){

    AssertExit(!mInitialized, "Display", "already initialized");

    // init st7789v
    mSt7789v.init(mWidth, mHeight);
    mSt7789v.setSendSyncHandler([this](const uint8_t* data, size_t numBytes){
        handleSt7789vSendSync(data, numBytes);
    });
    mSt7789v.setSendAsyncHandler([this](const uint8_t* data, size_t numBytes){
        handleSt7789vSendAsync(data, numBytes);
    });

    // send st7789v display init sequence
    mSt7789v.sendReset();
    mSt7789v.sendInit();

    // init lvgl handler
    mLvglHandler.init();
    for(const uint8_t* buffer : mLvglHandler.getDrawBufferPointers()){
        const DisplayBufferTracker::Result result = mBufferTracker.registerBuffer(buffer);
        AssertExit(result == DisplayBufferTracker::Result::Ok,
                   "Display", "failed to register draw buffer", static_cast<int32_t>(result));
    }

    // create semaphores
    constexpr UBaseType_t bufferCount = static_cast<UBaseType_t>(DisplayBufferTracker::Capacity);
    mBufferCounter = xSemaphoreCreateCounting(bufferCount, bufferCount);
    mRenderTrigger = xSemaphoreCreateBinary();
    mRenderReady = xSemaphoreCreateBinary();
    mDmaBarrier = xSemaphoreCreateBinary();
    mBufferTrackerMutex = xSemaphoreCreateMutex();

    AssertExit(mBufferCounter != nullptr, "Display", "failed to create buffer sem");
    AssertExit(mRenderTrigger != nullptr, "Display", "failed to create display mutex");
    AssertExit(mRenderReady != nullptr, "Display", "failed to create render-ready sem");
    AssertExit(mDmaBarrier != nullptr, "Display", "failed to create dma sem");
    AssertExit(mBufferTrackerMutex != nullptr, "Display", "failed to create buffer-tracker mutex");
    xSemaphoreGive(mRenderReady);

    // set handlers
    mLvglHandler.setFlushHandler([this](const RectXXYY<int32_t>& area, uint8_t* buffer){
        handleFlush(area, buffer);
    });
    mLvglHandler.setFlushWaitHandler([this](){
        handleFlushWait();
    });
    mRenderTask.setHandler([this](){
        this->handleRenderTask();
    });
    mSenderTask.setHandler([this](){
        this->handleSenderTask();
    });
    mChunkSender.setCompleteHandler([this](bool success){
        this->handleChunkComplete(success);
    });

    // create queue
    mSendQueue = xQueueCreate(DisplayBufferTracker::Capacity, sizeof(SendQueueEntry));
    AssertExit(mSendQueue != nullptr, "Display", "create queue failed");

    // initialization complete
    mInitialized = true;
}

void Display::startRenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t core){   
    // check state
    AssertExit(mInitialized, "Display", "not initialized");

    // start task
    mRenderTask.configure(name, stackSize, priority, core);
    mRenderTask.start();
}

void Display::startSenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t core){
    // check state
    AssertExit(mInitialized, "Display", "not initialized");

    // start task
    mSenderTask.configure(name, stackSize, priority, core);
    mSenderTask.start();
}

void Display::stopTasks(){
    {
        MutexGuard lock(mBufferTrackerMutex);
        mBufferTracker.stop();
        mActiveSendBuffer = nullptr;
    }

    if(mRenderTask.isRunning() && !mRenderTask.isCurrentTask()){
        mRenderTask.stop();
    }
    if(mSenderTask.isRunning() && !mSenderTask.isCurrentTask()){
        mSenderTask.stop();
    }

    // Buffers have static process lifetime, so an already queued hardware DMA
    // read remains memory-safe. Deselect the panel and reject all future reuse.
    mSt7789v.onSendAsyncComplete();
}

const Task& Display::getRenderTask() const {
    return mRenderTask;
}

const Task& Display::getSenderTask() const {
    return mSenderTask;
}

bool Display::tryTakeRenderReady(){
    {
        MutexGuard lock(mBufferTrackerMutex);
        if(mBufferTracker.lifecycle() != DisplayBufferTracker::Lifecycle::Running){
            return false;
        }
    }
    if(xSemaphoreTake(mRenderReady, 0) == pdTRUE){
        return true;
    }
    return false;
}

void Display::giveRenderTrigger(){
    {
        MutexGuard lock(mBufferTrackerMutex);
        if(mBufferTracker.lifecycle() != DisplayBufferTracker::Lifecycle::Running){
            return;
        }
    }
    xSemaphoreGive(mRenderTrigger);
}

void Display::handleRenderTask(){
    while(true){
        // wait for render trigger
        xSemaphoreTake(mRenderTrigger, portMAX_DELAY);

        // render frame
        Profiler::MeasureBegin(ProfilerId::DisplayRender);
        mLvglHandler.startRender();
        Profiler::MeasureEnd(ProfilerId::DisplayRender);        

        // signal render ready
        xSemaphoreGive(mRenderReady);
    }
}

void Display::handleFlush(const RectXXYY<int32_t>& rect, uint8_t* data){

    {
        MutexGuard lock(mBufferTrackerMutex);
        if(mBufferTracker.lifecycle() != DisplayBufferTracker::Lifecycle::Running){
            return;
        }
    }

    // first ensure buffer availability
    xSemaphoreTake(mBufferCounter, portMAX_DELAY);

    DisplayBufferTracker::Result queueResult;
    {
        MutexGuard lock(mBufferTrackerMutex);
        queueResult = mBufferTracker.queue(data);
    }
    if(queueResult != DisplayBufferTracker::Result::Ok){
        xSemaphoreGive(mBufferCounter);
        if(queueResult == DisplayBufferTracker::Result::NotRunning){
            return;
        }
        TriggerExit("Display", "invalid draw-buffer queue transition", static_cast<int32_t>(queueResult));
        return;
    }

    // debug logging
#if GarboxDebugDisplay
    if(pixelMap == mDrawBufferData1){
        LogDebug("Display", "handle flush buffer-1");
    }
    else if(pixelMap == mDrawBufferData2){
        LogDebug("Display", "handle flush buffer-2");
    }
    else if(pixelMap == mDrawBufferData3){
        LogDebug("Display", "handle flush buffer-3");
    }
    else {
        LogDebug("Display", "handle flush buffer-invalid");
    }
#endif
    
    // add to queue
    SendQueueEntry entry;
    entry.data = data;
    entry.sizeBytes = rect.width() * rect.height() * 2;
    entry.rect = rect;

    if(xQueueSend(mSendQueue, &entry, 0) != pdTRUE){
        DisplayBufferTracker::Result cancelResult;
        {
            MutexGuard lock(mBufferTrackerMutex);
            cancelResult = mBufferTracker.cancelQueued(data);
        }
        AssertExit(cancelResult == DisplayBufferTracker::Result::Ok,
                   "Display", "failed to roll back draw-buffer queue", static_cast<int32_t>(cancelResult));
        xSemaphoreGive(mBufferCounter);
        TriggerExit("Display", "flush queue is full");
        return;
    }
}

void Display::handleFlushWait(){
    {
        MutexGuard lock(mBufferTrackerMutex);
        if(mBufferTracker.lifecycle() != DisplayBufferTracker::Lifecycle::Running){
            return;
        }
    }
    xSemaphoreTake(mBufferCounter, portMAX_DELAY); // block until at least one DMA finishes (token returned)
    // now we are sure that at least one buffer is free to be used by LVGL
    xSemaphoreGive(mBufferCounter); // give token back immediately so next flush_cb can proceed
}

void Display::handleSenderTask(){
    SendQueueEntry entry;
    while(true){
        if(xQueueReceive(mSendQueue, &entry, portMAX_DELAY) == pdTRUE) {
            DisplayBufferTracker::Result dmaResult;
            {
                MutexGuard lock(mBufferTrackerMutex);
                dmaResult = mBufferTracker.beginDma(entry.data);
                if(dmaResult == DisplayBufferTracker::Result::Ok){
                    mActiveSendBuffer = entry.data;
                }
            }
            if(dmaResult != DisplayBufferTracker::Result::Ok){
                if(dmaResult == DisplayBufferTracker::Result::NotRunning){
                    continue;
                }
                TriggerExit("Display", "invalid draw-buffer DMA transition", static_cast<int32_t>(dmaResult));
                continue;
            }

            // send data to display via spi
            // the send command sends 3 spi sync commands, followed by 1 spi async (dma) command
            // the ST7789 requires this, which is why we can't directly queue DMA commands and
            // have to ping-pong between sending a flush buffer and dma completion
            const bool async = true;
            mSt7789v.sendDrawBufferXXYY(
                entry.rect.x1, entry.rect.x2, 
                entry.rect.y1, entry.rect.y2, 
                entry.data, entry.sizeBytes, 
                async
            );
            
            // wait until spi dma transfer is complete (ping)
            xSemaphoreTake(mDmaBarrier, portMAX_DELAY); 
        }
    }
}

void Display::handleChunkComplete(bool success){
    mSt7789v.onSendAsyncComplete();

    DisplayBufferTracker::Result result;
    bool transportFailed = false;
    {
        MutexGuard lock(mBufferTrackerMutex);
        if(mBufferTracker.lifecycle() != DisplayBufferTracker::Lifecycle::Running){
            mActiveSendBuffer = nullptr;
            return;
        }

        result = success
            ? mBufferTracker.completeDma(mActiveSendBuffer)
            : mBufferTracker.failDma(mActiveSendBuffer);
        if(result == DisplayBufferTracker::Result::Ok){
            mActiveSendBuffer = nullptr;
            transportFailed = !success;
        }
    }
    if(result != DisplayBufferTracker::Result::Ok){
        TriggerExit("Display", "invalid draw-buffer completion", static_cast<int32_t>(result));
        return;
    }

    // Always release the sender task before escalating a transport failure.
    // Normal completion additionally returns exactly one reusable LVGL token.
    if(transportFailed){
        xSemaphoreGive(mDmaBarrier);
        TriggerExit("Display", "pixel transport failed");
        return;
    }

    // return one free-buffer token
    xSemaphoreGive(mBufferCounter);
    // notify sender task that dma is complete and safe to start another spi call (pong)
    xSemaphoreGive(mDmaBarrier);
}

void Display::handleSt7789vSendSync(const uint8_t* data, size_t numBytes){
    mSpi.transferSync(data, numBytes*8);
}

void Display::handleSt7789vSendAsync(const uint8_t* data, size_t numBytes){
    mChunkSender.start(data, numBytes);
}

void Display::setBrightness(float brightness) {
    mSt7789v.setBrightness(brightness);
}

float Display::getBrightness() const {
    return mSt7789v.getBrightness();
}

}  // namespace Garbox
