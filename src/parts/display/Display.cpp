#include "Display.h"

#include <cmath>
#include <esp_heap_caps.h>

#include "assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/hardware/bus/SpiDma.h"
#include "core/time/Time.h"
#include "util/ByteUtils.h"
#include "util/color/types/Rgb565.h"

namespace Garbox {

#define GarboxDebugDisplay 1

Display::Display(const Config& config): 
    // init members 
    mSpi(config.spi),
    mSt7789v(config.gpioRst, config.gpioDc, config.pwmBlk),
    mWidth(config.width),
    mHeight(config.height),
    mBytesPerPixel(config.bytesPerPixel),
    mBufferPartialFactor(config.bufferPartialFactor),
    mBufferSize(config.bufferSizeBytes),
    mBufferWidth(config.bufferWidth),
    mBufferHeight(config.bufferHeight){

    // constructor body
    AssertExit(mWidth == mBufferWidth, "Display", "display and buffer width must be equal");
    AssertExit((mBufferHeight * mBufferPartialFactor) == mHeight, "Display", "inconsistent buffer and display height");
    AssertExit((mWidth / mBufferPartialFactor) > 0, "Display", "partial buffer height cannot be zero");
    AssertExit(mBytesPerPixel == 2, "Display", "RGB565 requires exactly 2 bytes per pixel");
    AssertExit(mBufferSize == (mWidth * mHeight * mBytesPerPixel / mBufferPartialFactor), "Display", "frame size inconsistent with constants");
}

void Display::init(){

    AssertExit(!mInitialized, "Display", "already initialized");

    // init st7789v
    mSt7789v.init(mWidth, mHeight);

    // register st7789v handlers
    mSt7789v.setSendSyncHandler([this](const uint8_t* data, size_t numBytes){
        handleSt7789vSendSync(data, numBytes);
    });
    mSt7789v.setSendAsyncHandler([this](const uint8_t* data, size_t numBytes){
        handleSt7789vSendAsync(data, numBytes);
    });

    // send display init sequence
    mSt7789v.sendReset();
    mSt7789v.sendInit();

    // initialize lvgl
    lv_init();

    // register callbacks
    lv_log_register_print_cb(handleLog);
    lv_tick_set_cb(lvglTickProvider);

    // init draw buffers
    mDrawBufferData1 = allocDrawBufferData(mBufferSize);
    mDrawBufferData2 = allocDrawBufferData(mBufferSize);
    mDrawBufferData3 = allocDrawBufferData(mBufferSize);
    initDrawBuffer(mDrawBuffer1, mDrawBufferData1, mBufferSize, mBufferWidth, mBufferHeight);
    initDrawBuffer(mDrawBuffer2, mDrawBufferData2, mBufferSize, mBufferWidth, mBufferHeight);
    initDrawBuffer(mDrawBuffer3, mDrawBufferData3, mBufferSize, mBufferWidth, mBufferHeight);

    // init display
    mLvDisplay = lv_display_create(mSt7789v.getWidth(), mSt7789v.getHeight());
    lv_display_set_user_data(mLvDisplay, this); 

    // setup rendering
    lv_display_set_draw_buffers(mLvDisplay, &mDrawBuffer1, &mDrawBuffer2);
    lv_display_set_3rd_draw_buffer(mLvDisplay, &mDrawBuffer3);
    lv_display_set_render_mode(mLvDisplay, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // flush callbacks
    lv_display_set_flush_cb(mLvDisplay, flushTrampoline);
    lv_display_set_flush_wait_cb(mLvDisplay, flushWaitTrampoline);

    // create label
    mLabel = lv_label_create(lv_screen_active());
    lv_label_set_text(mLabel, "Garbox Display");
    lv_obj_set_style_text_font(mLabel, &lv_font_unscii_16, LV_PART_MAIN);
    lv_obj_set_align(mLabel, lv_align_t::LV_ALIGN_TOP_MID);

    // send initial commands to display
    const Rgb565 clearColor = Rgb565::FromRgbFloat(0, 0, 0);
    mSt7789v.sendFillColor(clearColor.value);

    // create semaphores
    mBufferSem = xSemaphoreCreateCounting(3, 3); // 3 free buffers at startup
    mRenderSem = xSemaphoreCreateBinary();
    mDisplaySem = xSemaphoreCreateRecursiveMutex();

    AssertExit(mBufferSem != nullptr, "Display", "failed to create buffer sem");
    AssertExit(mRenderSem != nullptr, "Display", "failed to create render sem");
    AssertExit(mDisplaySem != nullptr, "Display", "failed to create display mutex");

    // initially free
    xSemaphoreGive(mRenderSem);

    // initialization complete
    mSt7789v.setBrightness(0.75f);
    mTestTimer.start(2000_ms);
    mInitialized = true;
}

void Display::startTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t core){
    LockGuard lock(mDisplaySem);
    
    // check state
    AssertExit(mInitialized, "Display", "not initialized");

    // start task
    mTask.configure(name, stackSize, priority, core);
    mTask.start(taskTrampoline, this);
}

void Display::stopTask(){
    LockGuard lock(mDisplaySem);
    mTask.stop();
    // TODO stop all spi activity
    // TODO leave display in a defined state after stopping
}

TaskHandle_t Display::getTaskHandle(){
    return mTask.getHandle();
}

void Display::handleTask(){
    while(true){
        // wait for render trigger
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 

        // execute display task
        Profiler::Begin(ProfilerId::DisplayTick);
        xSemaphoreTake(mRenderSem, portMAX_DELAY);
        lv_timer_handler();
        xSemaphoreGive(mRenderSem); // at this point, at least one buffer is free again (guaranteed by flush_wait_cb)
        Profiler::End(ProfilerId::DisplayTick);
    }
}

void Display::handleFlush(const lv_area_t* area, uint8_t* pixelMap){

    // first ensure buffer availability
    xSemaphoreTake(mBufferSem, portMAX_DELAY);

    // now access display state
    // this method is called from DisplayTask
    // state is modified through mFlushQueue
    LockGuard lock(mDisplaySem); // prevent concurrent access to display

    // debug logging
#if GarboxDebugDisplay
    if(pixelMap == mDrawBufferData1){
        LogDebug("Display", "buffer-1");
    }
    else if(pixelMap == mDrawBufferData2){
        LogDebug("Display", "buffer-2");
    }
    else if(pixelMap == mDrawBufferData3){
        LogDebug("Display", "buffer-3");
    }
    else {
        LogDebug("Display", "buffer-invalid");
    }
#endif
    
    // send buffer to st7789v
    const int32_t width = area->x2 - area->x1 + 1;
    const int32_t height = area->y2 - area->y1 + 1;
    const size_t sizeBytes = static_cast<size_t>(width * height * 2);

    // add to queue
    FlushQueueEntry* entry = mFlushQueue.pushPtr();
    if(entry == nullptr){
        TriggerExit("Display", "flush queue is full");
        return;
    }
    entry->data = pixelMap;
    entry->sizeBytes = sizeBytes;
    entry->x1 = area->x1;
    entry->x2 = area->x2;
    entry->y1 = area->y1;
    entry->y2 = area->y2;

    // send if nothing is being send currently
    sendNextInQueue();
}

void Display::handleFlushWait(){
    xSemaphoreTake(mBufferSem, portMAX_DELAY); // block until at least one DMA finishes (token returned)
    // now we are sure that at least one buffer is free to be used by LVGL
    xSemaphoreGive(mBufferSem); // give token back immediately so next flush_cb can proceed
}

void Display::handleDmaComplete(bool success){

    // this method is called from SpiDmaCompleteTask
    // state is modified through lv_display_flush_ready()
    LockGuard lock(mDisplaySem); // prevent concurrent access to display state

    // notify lvgl there is a free buffer
    lv_display_flush_ready(mLvDisplay); // tell LVGL the buffer is free again

    // return one free-buffer token
    xSemaphoreGive(mBufferSem); 

    // send next
    mSendInProgress = false;
    sendNextInQueue();
}

void Display::sendNextInQueue(){

    // this method can be called from:
    // - DisplayTask through lv_timer_handler() => handleFlush()
    // - SpiDmaTask  through handleDmaComplete()
    // state is modified through 
    // - mFlushQueue 
    // - mSendInProgress
    LockGuard lock(mDisplaySem); // prevent concurrent access to display state

    // check if something is already being sent
    if(mSendInProgress){
        return;
    }

    // get next entry in flush queue
    FlushQueueEntry* entry = mFlushQueue.popPtr();

    // send next entry
    if(entry != nullptr){
        const bool async = true;
        mSendInProgress = true;
        mSt7789v.sendDrawBufferXXYY(entry->x1, entry->x2, entry->y1, entry->y2, entry->data, entry->sizeBytes, async);
    }
}

void Display::handleSt7789vSendSync(const uint8_t* data, size_t numBytes){
    LockGuard lock(mDisplaySem); // prevent concurrent access to display state
    mSpi.transferSync(data, numBytes*8);
}

void Display::handleSt7789vSendAsync(const uint8_t* data, size_t numBytes){
    LockGuard lock(mDisplaySem); // prevent concurrent access to display state
    mSpi.transferAsync(data, numBytes*8, this, dmaCompleteTrampoline);
}

void Display::handleLog(lv_log_level_t level, const char* str){
    switch(level){
    case LV_LOG_LEVEL_TRACE:
        LogDebug0("LVGL/Trace", "%s", str);
        break;
    case LV_LOG_LEVEL_INFO:
        LogInfo0("LVGL/Info", "%s", str);
        break;
    case LV_LOG_LEVEL_WARN:
        LogWarning0("LVGL/Warn", "%s", str);
        TriggerDebug("Display", "LVGL Warning");
        break;
    case LV_LOG_LEVEL_ERROR:
        LogError0("LVGL/Error", "%s", str);
        TriggerExit("Display", "LVGL Error");
        break;
    case LV_LOG_LEVEL_USER:        
    default:
        TriggerDebug("Display", "unhandled log level");
        break;
    }
}

void Display::taskTrampoline(void* user){
    AssertExit(user != nullptr, "Display", "user is nullptr");
    static_cast<Display*>(user)->handleTask();   
}

void Display::flushTrampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* pixelMap){
    AssertExit(disp != nullptr, "Display", "disp is nullptr");
    Display* self = static_cast<Display*>(lv_display_get_user_data(disp));
    if (self){
        self->handleFlush(area, pixelMap);
    }
}

void Display::flushWaitTrampoline(lv_display_t* disp){
    AssertExit(disp != nullptr, "Display", "disp is nullptr");
    Display* self = static_cast<Display*>(lv_display_get_user_data(disp));
    if (self){
        self->handleFlushWait();
    }
}

void Display::dmaCompleteTrampoline(void* user, bool success){
    AssertExit(user != nullptr, "Display", "user is nullptr");
    static_cast<Display*>(user)->handleDmaComplete(success);
}

uint32_t Display::lvglTickProvider(){
    return Time::GetMillis();
}

uint8_t* Display::allocDrawBufferData(uint32_t size){
    // alloc memory
    uint8_t* buffer = (uint8_t*) heap_caps_malloc(size, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    
    // check if alloc success
    AssertExit(buffer != nullptr, "Display", "mDrawBuffer is nullptr");
    if (!(heap_caps_get_allocated_size(buffer) && heap_caps_get_largest_free_block(MALLOC_CAP_DMA))){
        multi_heap_info_t info;
        heap_caps_get_info(&info, MALLOC_CAP_DMA);
        TriggerExit("Display", "DMA allocation failed", static_cast<int32_t>(info.total_free_bytes));
    }

    // pointer to buffer
    return buffer;
}

void Display::initDrawBuffer(lv_draw_buf_t& buffer, uint8_t* data, uint32_t size, uint32_t width, uint32_t height){
    buffer.header.cf = LV_COLOR_FORMAT_RGB565;      
    buffer.header.w = width;
    buffer.header.h = height;
    buffer.header.stride = width * sizeof(lv_color_t);
    buffer.data = data;
    buffer.data_size = size;
    buffer.unaligned_data = data;
    buffer.handlers = nullptr;
}

}  // namespace Garbox
