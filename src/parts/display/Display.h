#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <lvgl.h>

#include "core/hardware/bus/SpiDmaChunkSender.h"
#include "core/rtos/Task.h"
#include "core/time/SoftwareTimer.h"
#include "parts/display/St7789v.h"

namespace Garbox {

class LedcChannel;
class Gpio;
class SpiDma;

class Display {
public:

    struct Config {
        SpiDma& spi;
        Gpio& gpioRst;
        Gpio& gpioDc;
        Gpio& gpioCs;
        LedcChannel& pwmBlk;
        uint32_t width;
        uint32_t height;
        uint32_t bytesPerPixel;
        uint32_t bufferPartialFactor;
        uint32_t bufferSizeBytes;
        uint32_t bufferWidth;
        uint32_t bufferHeight;
        uint32_t chunkSizeBytes = 1024*4;
    };

    Display(const Config& config);

    void init();

    void startRenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void startSenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void stopTasks();

    bool tryTakeRenderReady();
    void giveRenderTrigger();

    // Disallow copy and move 
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;

private:

    struct SendQueueEntry {
        uint8_t* data = nullptr;
        size_t sizeBytes = 0;  
        uint16_t x1 = 0;
        uint16_t x2 = 0;
        uint16_t y1 = 0;
        uint16_t y2 = 0;
    };

    SpiDma& mSpi;
    St7789v mSt7789v;    
    SpiDmaChunkSender mChunkSender;

    SoftwareTimer mTestTimer;
    QueueHandle_t mSendQueue;

    // state
    bool mInitialized = false;
    bool mSendInProgress = false;
    uint32_t mRenderSkipCount = 0;

    // lvgl display configuration
    const uint32_t mWidth;
    const uint32_t mHeight;
    const uint32_t mBytesPerPixel;
    const uint32_t mBufferPartialFactor;
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
    lv_obj_t* mBackground;
    lv_obj_t* mBox1;
    lv_obj_t* mBox2;
    lv_obj_t* mBox3;
    lv_obj_t* mBox4;

    // task
    Task mRenderTask;
    Task mSenderTask;

    // semaphore counts how many draw buffers are free (3 for triple buffering)
    SemaphoreHandle_t mBufferCounter;
    
    // display render barrier
    SemaphoreHandle_t mRenderTrigger;
    
    // display render barrier
    SemaphoreHandle_t mRenderReady;
    
    // dma semaphore
    SemaphoreHandle_t mDmaBarrier;

    // callback handlers
    void handleRenderTask();
    void handleSenderTask();
    void handleFlush(const lv_area_t* area, uint8_t* px_map);
    void handleFlushWait();
    void handleChunkComplete(bool success);

    // callback trampolines
    static void renderTaskTrampoline(void* user);
    static void senderTaskTrampoline(void* user);
    static void flushTrampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
    static void flushWaitTrampoline(lv_display_t* disp);
    static void chunkCompleteTrampoline(void* user, bool success);

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
