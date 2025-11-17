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
#include "util/color/types/Rgb888.h"
#include "util/color/types/HslColor.h"

namespace Garbox {

#define GarboxDebugDisplay 0

Display::Display(const Config& config): 
    // init members 
    mSpi(config.spi),
    mSt7789v(config.gpioRst, config.gpioDc, config.gpioCs, config.pwmBlk),
    mChunkSender(mSpi, config.chunkSizeBytes),
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

    // create background
    mBackground = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBackground, mWidth, mHeight);
    lv_obj_set_style_bg_color(mBackground, lv_color_hex(0x0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBackground, LV_OPA_COVER, LV_PART_MAIN);

    // create background
    mBox1 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBox1, 48, 48);
    lv_obj_set_style_bg_color(mBox1, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBox1, LV_OPA_COVER, LV_PART_MAIN);

    // create background
    mBox2 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBox2, 48, 48);
    lv_obj_set_style_bg_color(mBox2, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBox2, LV_OPA_COVER, LV_PART_MAIN);

    // create background
    mBox3 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBox3, 48, 48);
    lv_obj_set_style_bg_color(mBox3, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBox3, LV_OPA_COVER, LV_PART_MAIN);

    // create background
    mBox4 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBox4, 48, 48);
    lv_obj_set_style_bg_color(mBox4, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBox4, LV_OPA_COVER, LV_PART_MAIN);

    // create label
    mLabel = lv_label_create(lv_screen_active());
    lv_label_set_text(mLabel, "Garbox Display");
    lv_obj_set_style_text_color(mLabel, lv_color_hex(0xFF7F00), LV_PART_MAIN);
    lv_obj_set_style_text_font(mLabel, &lv_font_unscii_16, LV_PART_MAIN);
    lv_obj_set_align(mLabel, lv_align_t::LV_ALIGN_CENTER);

    // create semaphores
    mBufferCounter = xSemaphoreCreateCounting(3, 3); // 3 free buffers at startup
    mRenderTrigger = xSemaphoreCreateBinary();
    mRenderReady = xSemaphoreCreateBinary();
    mDmaBarrier = xSemaphoreCreateBinary();
    xSemaphoreGive(mRenderReady);

    AssertExit(mBufferCounter != nullptr, "Display", "failed to create buffer sem");
    AssertExit(mRenderTrigger != nullptr, "Display", "failed to create display mutex");
    AssertExit(mDmaBarrier != nullptr, "Display", "failed to create dma sem");

    // create queue
    mSendQueue = xQueueCreate(3, sizeof(SendQueueEntry));
    AssertDebug(mSendQueue != nullptr, "Display", "create queue failed");

    // initialization complete
    mSt7789v.setBrightness(0.75f);
    mTestTimer.start(2000_ms);
    mInitialized = true;
}

void Display::startRenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t core){   
    // check state
    AssertExit(mInitialized, "Display", "not initialized");

    // start task
    mRenderTask.configure(name, stackSize, priority, core);
    mRenderTask.start(renderTaskTrampoline, this);
}

void Display::startSenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t core){
    // check state
    AssertExit(mInitialized, "Display", "not initialized");

    // start task
    mSenderTask.configure(name, stackSize, priority, core);
    mSenderTask.start(senderTaskTrampoline, this);
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

        // advance t
        static float t = 0.0f;
        t = std::fmod(t + 0.001f, 1.0f);
        lv_label_set_text_fmt(mLabel, "0.%03u", static_cast<uint32_t>(t*1000.0f));

        static float x = 320.0f/2 + 48.0f/2;
        static float y = 240.0f/2 + 48.0f/2;
        static float xdir = 0.8f;
        static float ydir = 0.8f;
        x += xdir;
        y += ydir;
        static float maxx = 320.0f-48.0f;
        static float maxy = 240.0f-48.0f;
        if(x > maxx){
            xdir = -xdir;
            x = maxx;
        }
        else if (x < 0.0f){
            xdir = -xdir;
            x = 0.0f;
        }
        if(y > maxy){
            ydir = -ydir;
            y = maxy;
        }
        else if(y < 0.0f){
            ydir = -ydir;
            y = 0.0f;
        }
        
        lv_obj_set_pos(mBox1, x, y);
        lv_obj_set_pos(mBox2, maxx-x, y);
        lv_obj_set_pos(mBox3, x, maxy-y);
        lv_obj_set_pos(mBox4, maxx-x, maxy-y);

        // render frame
        Profiler::Begin(ProfilerId::DisplayTick);
        lv_timer_handler();
        Profiler::End(ProfilerId::DisplayTick);

        xSemaphoreGive(mRenderReady);
    }
}

void Display::handleFlush(const lv_area_t* area, uint8_t* pixelMap){

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
    
    // send buffer to st7789v
    const int32_t width = area->x2 - area->x1 + 1;
    const int32_t height = area->y2 - area->y1 + 1;
    const size_t sizeBytes = static_cast<size_t>(width * height * 2);

    // add to queue
    SendQueueEntry entry;
    entry.data = pixelMap;
    entry.sizeBytes = sizeBytes;
    entry.x1 = area->x1;
    entry.x2 = area->x2;
    entry.y1 = area->y1;
    entry.y2 = area->y2;

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
            mSt7789v.sendDrawBufferXXYY(entry.x1, entry.x2, entry.y1, entry.y2, entry.data, entry.sizeBytes, async);
            
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
    mChunkSender.start(data, numBytes, this, chunkCompleteTrampoline);
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

void Display::renderTaskTrampoline(void* user){
    AssertExit(user != nullptr, "Display", "user is nullptr");
    static_cast<Display*>(user)->handleRenderTask();   
}

void Display::senderTaskTrampoline(void* user){
    AssertExit(user != nullptr, "Display", "user is nullptr");
    static_cast<Display*>(user)->handleSenderTask();   
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

void Display::chunkCompleteTrampoline(void* user, bool success){
    AssertExit(user != nullptr, "Display", "user is nullptr");
    static_cast<Display*>(user)->handleChunkComplete(success);
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
