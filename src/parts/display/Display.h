#pragma once

#include "core/hardware/gpio/Gpio.h"
#include "core/time/SoftwareTimer.h"
#include "global/AppConfig.h"
#include "lvgl.h"

namespace Garbox {

class SpiDma;
class LedcChannel;
class Gpio;

class Display {
public:

    Display();

    void init();
    void tick();
    void txTick();

private:

    /* Instance members */
    SpiDma& mSpi;
    LedcChannel& mBacklightPwm;
    Gpio& mGpioRst;
    Gpio& mGpioDc;
    const uint16_t mWidth;
    const uint16_t mHeight;
    const uint32_t mBufferSize;
    SoftwareTimer mTestTimer;
    lv_display_t* mLvDisplay = nullptr;
    bool mInitialized = false;

    /* Drawing buffer */
    uint16_t* mDrawBufA = nullptr;
    uint16_t* mDrawBufB = nullptr;
    volatile bool mFlushing = false;
    uint32_t mRenderSkipCount = 0;

    void hardwareInit();
    void handleFlush(const lv_area_t* area, uint8_t* px_map);
    void handleTxComplete(bool success);
    static void handleLog(lv_log_level_t level, const char* buf);
    static uint32_t tickCallback();
    static void flushTrampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
    static void txCompleteTrampoline(void* user, bool success);

    void testFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

};

} // namespace Garbox
