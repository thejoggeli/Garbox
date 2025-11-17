#include "Display.h"

#include <cmath>

#include "assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/hardware/bus/SpiDma.h"
#include "util/ByteUtils.h"
#include "util/color/types/Rgb565.h"
#include "util/color/types/Rgb888.h"
#include "util/color/types/HslColor.h"

namespace Garbox {

#define GarboxDebugDisplay 0

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
    mSt7789v.setBrightness(0.75f);

    // init lvgl handler
    mLvglHandler.init();

    // create semaphores
    mBufferCounter = xSemaphoreCreateCounting(3, 3); // 3 free buffers at startup
    mRenderTrigger = xSemaphoreCreateBinary();
    mRenderReady = xSemaphoreCreateBinary();
    mDmaBarrier = xSemaphoreCreateBinary();
    xSemaphoreGive(mRenderReady);

    AssertExit(mBufferCounter != nullptr, "Display", "failed to create buffer sem");
    AssertExit(mRenderTrigger != nullptr, "Display", "failed to create display mutex");
    AssertExit(mDmaBarrier != nullptr, "Display", "failed to create dma sem");

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
    mSendQueue = xQueueCreate(3, sizeof(SendQueueEntry));
    AssertDebug(mSendQueue != nullptr, "Display", "create queue failed");

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
    mRenderTask.stop();
    mSenderTask.stop();
    // TODO stop all spi activity
    // TODO leave display in a defined state after stopping
}

bool Display::tryTakeRenderReady(){
    if(xSemaphoreTake(mRenderReady, 0) == pdTRUE){
        return true;
    }
    return false;
}

void Display::giveRenderTrigger(){
    xSemaphoreGive(mRenderTrigger);
}

void Display::handleRenderTask(){
    while(true){
        // wait for render trigger
        xSemaphoreTake(mRenderTrigger, portMAX_DELAY);

        // render frame
        Profiler::Begin(ProfilerId::DisplayTick);
        mLvglHandler.startRender();
        Profiler::End(ProfilerId::DisplayTick);

        // signal render ready
        xSemaphoreGive(mRenderReady);
    }
}

void Display::handleFlush(const RectXXYY<int32_t>& rect, uint8_t* data){

    // first ensure buffer availability
    xSemaphoreTake(mBufferCounter, portMAX_DELAY);

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
        TriggerExit("Display", "flush queue is full");
        return;
    }
}

void Display::handleFlushWait(){
    xSemaphoreTake(mBufferCounter, portMAX_DELAY); // block until at least one DMA finishes (token returned)
    // now we are sure that at least one buffer is free to be used by LVGL
    xSemaphoreGive(mBufferCounter); // give token back immediately so next flush_cb can proceed
}

void Display::handleSenderTask(){
    SendQueueEntry entry;
    while(true){
        if(xQueueReceive(mSendQueue, &entry, portMAX_DELAY) == pdTRUE) {
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

}  // namespace Garbox
