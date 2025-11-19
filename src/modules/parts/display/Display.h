#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <lvgl.h>

#include "core/hardware/bus/SpiDmaChunkSender.h"
#include "core/rtos/Task.h"
#include "modules/parts/display/LvglHandler.h"
#include "modules/parts/display/St7789v.h"
#include "core/util/math/RectXXYY.h"

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
        uint32_t partialFactor;
        uint32_t chunkSizeBytes;
    };

    Display(const Config& config);

    LvglHandler& getLvglHandler();

    void init();

    void startRenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void startSenderTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void stopTasks();
    const Task& getRenderTask() const;
    const Task& getSenderTask() const;

    bool tryTakeRenderReady();
    void giveRenderTrigger();

    // disallow copy and move 
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;

private:

    struct SendQueueEntry {
        uint8_t* data = nullptr;
        size_t sizeBytes = 0;  
        RectXXYY<int32_t> rect;
    };

    SpiDma& mSpi;
    St7789v mSt7789v;    
    LvglHandler mLvglHandler;
    SpiDmaChunkSender mChunkSender;

    // state
    bool mInitialized = false;

    // lvgl display configuration
    const uint32_t mWidth;
    const uint32_t mHeight;

    // tasks
    Task mRenderTask; // runs lvgl rendering
    Task mSenderTask; // worker that sends pixeldata to display via SPI

    // queue for sender task
    QueueHandle_t mSendQueue;

    // semaphores
    SemaphoreHandle_t mBufferCounter; // semaphore counts how many draw buffers are free (3 for triple buffering)
    SemaphoreHandle_t mRenderTrigger; // display render barrier
    SemaphoreHandle_t mRenderReady; // display render barrier
    SemaphoreHandle_t mDmaBarrier; // dma semaphore

    // callback handlers
    void handleFlush(const RectXXYY<int32_t>& rect, uint8_t* data);
    void handleFlushWait();
    void handleRenderTask();
    void handleSenderTask();
    void handleChunkComplete(bool success);

    // st7789v handlers
    void handleSt7789vSendSync(const uint8_t* data, size_t numBytes);
    void handleSt7789vSendAsync(const uint8_t* data, size_t numBytes);
    
};

} // namespace Garbox
