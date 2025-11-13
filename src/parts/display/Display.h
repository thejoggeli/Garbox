#pragma once

#include "core/time/SoftwareTimer.h"
#include "global/config/AppConfig.h"
#include "lvgl.h"
#include "St7789v.h"

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

    const uint32_t mBufferSize;
    lv_display_t* mLvDisplay = nullptr;
    bool mInitialized = false;

    /* Drawing buffer */
    uint8_t* mDrawBuffer1 = nullptr;
    uint8_t* mDrawBuffer2 = nullptr;
    uint32_t mRenderSkipCount = 0;

    lv_obj_t* mLabel;

    uint8_t* allocDrawBuffer();

    void handleFlush(const lv_area_t* area, uint8_t* px_map);
    void handleTxComplete(bool success);
    static void handleLog(lv_log_level_t level, const char* buf);
    static uint32_t lvglTickProvider();
    static void flushTrampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
    static void txCompleteTrampoline(void* user, bool success);

    void handleSt7789vSendSync(const uint8_t* data, size_t numBytes);
    void handleSt7789vSendAsync(const uint8_t* data, size_t numBytes);
    
};

} // namespace Garbox
