#include "Display.h"

#include <cmath>
#include "assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/bus/SpiInstances.h"
#include "global/color/ColorMaps.h"
#include "global/gpio/GpioInstances.h"
#include "global/ledc/LedcInstances.h"
#include "util/ByteUtils.h"
#include "util/color/Rgb565.h"

namespace Garbox {

Display::Display(): 
    // init members 
    mSpi(SpiInstances::GetSpiDma()),
    mSt7789v(
        GpioInstances::GetDisplayRst(),
        GpioInstances::GetDisplayDc(),
        LedcInstances::GetBacklightChannel()
    ),
    mBufferSize(AppConfig::SpiDmaMaxTransferSizeBytes){
    // nothing to do
}

void Display::init() {

    AssertExit(!mInitialized, "Display", "already initialized");

    // init st7789v
    mSt7789v.init(AppConfig::DisplayWidth, AppConfig::DisplayHeight);
    mSt7789v.reset();

    // register st7789v handlers
    mSt7789v.setSendSyncHandler([&](const uint8_t* data, size_t numBytes){
        handleSt7789vSendSync(data, numBytes);
    });
    mSt7789v.setSendAsyncHandler([&](const uint8_t* data, size_t numBytes){
        handleSt7789vSendAsync(data, numBytes);
    });

    // send display init sequence
    mSt7789v.sendInitSequence();

    // initialize lvgl
    lv_init();

    // register callbacks
    lv_log_register_print_cb(handleLog);
    lv_tick_set_cb(lvglTickProvider);

    // init draw buffer
    mDrawBuffer = (uint16_t*) heap_caps_malloc(mBufferSize, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    AssertExit(mDrawBuffer != nullptr, "Display", "mDrawBuffer is nullptr");

    // init display
    mLvDisplay = lv_display_create(mSt7789v.getWidth(), mSt7789v.getHeight());
    lv_display_set_user_data(mLvDisplay, this); 
    lv_display_set_buffers(mLvDisplay, mDrawBuffer, nullptr, mBufferSize, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(mLvDisplay, flushTrampoline);

    lv_obj_t* label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Garbox Display");
    lv_obj_center(label);

    // send init commands to display
    mSt7789v.sendFillColor(Rgb565::fromFloat(1.0f, 0.0f, 1.0f).value);
    mSt7789v.sendFillRect(5,   5, 50, 50, Rgb565::fromFloat(1, 0, 0).value);
    mSt7789v.sendFillRect(60,  5, 50, 50, Rgb565::fromFloat(0, 1, 0).value);
    mSt7789v.sendFillRect(115, 5, 50, 50, Rgb565::fromFloat(0, 0, 1).value);

    // initialization complete
    mSt7789v.setBrightness(0.75f);
    mTestTimer.start(2000_ms);
    mInitialized = true;

}

void Display::tick() {

    // lv_timer_handler(); 

    // advance x
    static int x = 0;
    x = (x + 1) % 320;
    
    // advance t
    static float t = 0.0f;
    t = std::fmod(t + 0.005f, 1.0f);

    // draw rectangle with hsl rainbow
    Rgb565 rgb1 = Rgb565::fromHsl(t, 1.0f, 0.5f);
    mSt7789v.sendFillRect(x, 100, 1, 40, rgb1.value);

    // draw rectangle with colormap (rgb interpolation)
    Rgb565 rgb2 = Rgb565::fromFloat(ColorMaps::Test.interpolateRgb(t));
    mSt7789v.sendFillRect(x, 150, 1, 40, rgb2.value);

    // draw rectangle with colormap (hsl interpolation)
    Rgb565 rgb3 = Rgb565::fromHsl(ColorMaps::Test.interpolateHsl(t));
    mSt7789v.sendFillRect(x, 200, 1, 40, rgb3.value);

    // test timer
    if (mTestTimer.isExpired()) {
        mTestTimer.restart();
    }

}

void Display::handleFlush(const lv_area_t* area, uint8_t* pixelMap) {

    return;

    mFlushing = true;
    lv_display_flush_ready(mLvDisplay);

    uint32_t const width = area->x2 - area->x1 + 1;
    uint32_t const height = area->y2 - area->y1 + 1;
    uint32_t const sizeBytes = width * height * 2;
    mSt7789v.sendDrawBufferXXYY(area->x1, area->x2, area->y1, area->y2, pixelMap, sizeBytes, false);

    // notify lvgl
    lv_display_flush_ready(mLvDisplay);
}

void Display::handleTxComplete(bool success) {
    // nothing to do
}

uint32_t Display::lvglTickProvider() {
    return Time::GetMillisSlow();
}

void Display::handleLog(lv_log_level_t level, const char* str){
    switch(level){
        case LV_LOG_LEVEL_TRACE:
            LogDebug("LVGL/Trace", "%s", str);
            break;
        case LV_LOG_LEVEL_INFO:
            LogInfo("LVGL/Info", "%s", str);
            break;
        case LV_LOG_LEVEL_WARN:
            LogWarning("LVGL/Warn", "%s", str);
            TriggerDebug("Display", "LVGL Warning");
            break;
        case LV_LOG_LEVEL_ERROR:
            LogError("LVGL/Error", "%s", str);
            TriggerExit("Display", "LVGL Error");
            break;
        case LV_LOG_LEVEL_USER:        
        default:
            TriggerDebug("Display", "unhandled log level");
            break;
    }
}

void Display::flushTrampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* pixelMap){
    Display* self = static_cast<Display*>(lv_display_get_user_data(disp));
    if (self){
        self->handleFlush(area, pixelMap);
    }
}

void Display::txCompleteTrampoline(void* user, bool success) {
    static_cast<Display*>(user)->handleTxComplete(success);
}

void Display::handleSt7789vSendSync(const uint8_t* data, size_t numBytes){
    mSpi.transferSync(data, numBytes*8);
}

void Display::handleSt7789vSendAsync(const uint8_t* data, size_t numBytes){
    mSpi.transferSync(data, numBytes*8);
}

}  // namespace Garbox
