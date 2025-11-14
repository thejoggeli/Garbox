#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <lvgl.h>

#include "core/time/SoftwareTimer.h"
#include "global/config/AppConfig.h"
#include "parts/display/St7789v.h"

namespace Garbox {

class LedcChannel;
class Gpio;
class SpiDma;

class Display {
public:

    Display(SpiDma& spi, Gpio& gpioRst, Gpio& gpioDc, LedcChannel& pwmBlk);

    void init();
    void tick();

    // Disallow copy and move 
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;

private:

    /* Instance members */
    SpiDma& mSpi;
    St7789v mSt7789v;
    
    SoftwareTimer mTestTimer;

    const uint32_t mWidth;
    const uint32_t mHeight;
    const uint32_t mPartialFactor;
    const uint32_t mBytesPerPixel;
    const uint32_t mBufferSize;
    const uint32_t mBufferWidth;
    const uint32_t mBufferHeight;

    lv_display_t* mLvDisplay = nullptr;
    bool mInitialized = false;

    /* Drawing buffer */
    lv_draw_buf_t mDrawBuffer1;
    lv_draw_buf_t mDrawBuffer2;
    lv_draw_buf_t mDrawBuffer3;
    uint8_t* mDrawBufferData1 = nullptr;
    uint8_t* mDrawBufferData2 = nullptr;
    uint8_t* mDrawBufferData3 = nullptr;
    uint32_t mRenderSkipCount = 0;

    lv_obj_t* mLabel;

    void handleFlush(const lv_area_t* area, uint8_t* px_map);
    void handleFlushWait();
    void handleTxComplete(bool success);

    static void flushTrampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
    static void flushWaitTrampoline(lv_display_t* disp);
    static void txCompleteTrampoline(void* user, bool success);

    void handleSt7789vSendSync(const uint8_t* data, size_t numBytes);
    void handleSt7789vSendAsync(const uint8_t* data, size_t numBytes);

    static void handleLog(lv_log_level_t level, const char* buf);
    static uint32_t lvglTickProvider();


    static uint8_t* allocDrawBufferData(uint32_t size);
    static void initDrawBuffer(lv_draw_buf_t& buffer, uint8_t* data, uint32_t size, uint32_t width, uint32_t height);
    
};

} // namespace Garbox
