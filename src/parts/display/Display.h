#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <lvgl.h>

#include "core/time/SoftwareTimer.h"
#include "global/config/AppConfig.h"
#include "parts/display/St7789v.h"
#include "util/container/RingBuffer.h"
#include "util/threading/LockGuard.h"

namespace Garbox {

class LedcChannel;
class Gpio;
class SpiDma;

class Display {
public:

    Display(SpiDma& spi, Gpio& gpioRst, Gpio& gpioDc, LedcChannel& pwmBlk);

    void init();

    void startTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void stopTask();
    TaskHandle_t getTaskHandle();

    // Disallow copy and move 
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;

private:

    struct FlushQueueEntry {
        uint8_t* data = nullptr;
        size_t sizeBytes = 0;  
        uint16_t x1 = 0;
        uint16_t x2 = 0;
        uint16_t y1 = 0;
        uint16_t y2 = 0;
    };

    SpiDma& mSpi;
    St7789v mSt7789v;    
    SoftwareTimer mTestTimer;
    RingBuffer<FlushQueueEntry, 3> mFlushQueue;

    // state
    bool mInitialized = false;
    bool mSendInProgress = false;
    uint32_t mRenderSkipCount = 0;

    // lvgl display configuration
    const uint32_t mWidth;
    const uint32_t mHeight;
    const uint32_t mPartialFactor;
    const uint32_t mBytesPerPixel;
    const uint32_t mBufferSize;
    const uint32_t mBufferWidth;
    const uint32_t mBufferHeight;

    // drawing buffer
    lv_draw_buf_t mDrawBuffer1;
    lv_draw_buf_t mDrawBuffer2;
    lv_draw_buf_t mDrawBuffer3;
    uint8_t* mDrawBufferData1 = nullptr;
    uint8_t* mDrawBufferData2 = nullptr;
    uint8_t* mDrawBufferData3 = nullptr;

    // lvgl objects
    lv_display_t* mLvDisplay = nullptr;
    lv_obj_t* mLabel;

    // task
    TaskHandle_t mTaskHandle = nullptr;

    // semaphore counts how many draw buffers are free (3 for triple buffering)
    SemaphoreHandle_t mBufferSem;
    
    // binary semaphore (mutex-style) to serialize lv_timer_handler() calls
    SemaphoreHandle_t mRenderSem;
    
    // dislay state access
    SemaphoreHandle_t mDisplaySem;

    // callback handlers
    void handleTask();
    void handleFlush(const lv_area_t* area, uint8_t* px_map);
    void handleFlushWait();
    void handleDmaComplete(bool success);

    // flush queue
    void sendNextInQueue();

    // callback trampolines
    static void taskTrampoline(void* user);
    static void flushTrampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
    static void flushWaitTrampoline(lv_display_t* disp);
    static void dmaCompleteTrampoline(void* user, bool success);

    // st7789v handlers
    void handleSt7789vSendSync(const uint8_t* data, size_t numBytes);
    void handleSt7789vSendAsync(const uint8_t* data, size_t numBytes);

    // lvgl helpers
    static void handleLog(lv_log_level_t level, const char* buf);
    static uint32_t lvglTickProvider();

    // buffer helpers
    static uint8_t* allocDrawBufferData(uint32_t size);
    static void initDrawBuffer(lv_draw_buf_t& buffer, uint8_t* data, uint32_t size, uint32_t width, uint32_t height);
    
};

} // namespace Garbox
