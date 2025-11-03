#include "Display.h"

#include <cmath>
#include "assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/bus/SpiInstances.h"
#include "global/color/ColorMaps.h"
#include "global/gpio/GpioInstances.h"
#include "global/ledc/LedcInstances.h"
#include "St7789vHelper.h"
#include "util/ByteUtils.h"
#include "util/color/Rgb565.h"

namespace Garbox {

Display::Display(): 
    // init members    
    mSpi(SpiInstances::GetSpiDma()),
    mBacklightPwm(LedcInstances::GetBacklightChannel()),
    mGpioRst(GpioInstances::GetDisplayRst()),
    mGpioDc(GpioInstances::GetDisplayDc()),
    mWidth(AppConfig::DisplayWidth),
    mHeight(AppConfig::DisplayHeight),
    mBufferSize(AppConfig::SpiDmaMaxTransferSizeBytes),
    mTestTimer(){
    // nothing to do
}

void Display::init() {

    AssertExit(!mInitialized, "Display::init()", "already initialized");

    // initialize lvgl
    lv_init();

    // register log callback
    lv_log_register_print_cb(handleLog);

    // register tick callback
    lv_tick_set_cb(tickCallback);

    // init draw buffer
    mDrawBuf = (uint16_t*) heap_caps_malloc(mBufferSize, MALLOC_CAP_DMA);
    AssertExit(mDrawBuf != nullptr, "Display::init()", "mDrawBuf is nullptr");

    // init display
    mLvDisplay = lv_display_create(mWidth, mHeight);
    lv_display_set_user_data(mLvDisplay, this); 
    lv_display_set_buffers(mLvDisplay, mDrawBuf, nullptr, mBufferSize, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(mLvDisplay, flushTrampoline);

    lv_obj_t* label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Garbox Display");
    lv_obj_center(label);

    // send init commands to display
    hardwareInit();
    testFillRect(0, 0, mWidth, mHeight, 0);
    testFillRect(5,   5, 50, 50, Rgb565::fromFloat(1, 0, 0).value);
    testFillRect(60,  5, 50, 50, Rgb565::fromFloat(0, 1, 0).value);
    testFillRect(115, 5, 50, 50, Rgb565::fromFloat(0, 0, 1).value);

    // initialization complete
    mBacklightPwm.setDutyRelative(0.75f);
    mTestTimer.start(2000_ms);
    mInitialized = true;
}

void Display::hardwareInit(){

    // reset display
    mGpioRst.setValue(false);
    Time::DelayMicros(20_ms);
    mGpioRst.setValue(true);
    Time::DelayMicros(120_ms);

    uint8_t cmd;

    // sleep out
    cmd = St7789vHelper::SLPOUT;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
    Time::DelayMicros(120_ms);

    // color mode: 16-bit
    uint8_t colmod[] = { 
        St7789vHelper::COLMOD, 
        St7789vHelper::makeColmodValue(St7789vHelper::ColorMode::RGB565) 
    };
    mGpioDc.setValue(false);
    mSpi.transferSync(colmod, 8);
    mGpioDc.setValue(true);
    mSpi.transferSync(colmod + 1, 8);

    // memory access control (rotation, RGB order)
    uint8_t madctl[] = { 
        St7789vHelper::MADCTL, 
        St7789vHelper::LandscapeRGB(false, true) 
    };
    mGpioDc.setValue(false);
    mSpi.transferSync(madctl, 8);
    mGpioDc.setValue(true);
    mSpi.transferSync(madctl + 1, 8);

    // inversion on
    cmd = St7789vHelper::INVON;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);

    // display on
    cmd = St7789vHelper::DISPON;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
}

void Display::testFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
    uint8_t cmd;
    uint8_t data[4];

    // Column address set
    cmd = St7789vHelper::CASET;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
    mGpioDc.setValue(true);
    data[0] = x >> 8;
    data[1] = x & 0xFF;
    data[2] = (x + w - 1) >> 8;
    data[3] = (x + w - 1) & 0xFF;
    mSpi.transferSync(data, 32);

    // Row address set
    cmd = St7789vHelper::RASET;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
    mGpioDc.setValue(true);
    data[0] = y >> 8;
    data[1] = y & 0xFF;
    data[2] = (y + h - 1) >> 8;
    data[3] = (y + h - 1) & 0xFF;
    mSpi.transferSync(data, 32);

    // Memory write
    cmd = St7789vHelper::RAMWR;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
    mGpioDc.setValue(true);

    // Fill with the chosen color
    uint32_t const pixelCount = (uint32_t)w * h;
    constexpr size_t blockPixels = 64;
    uint16_t block[blockPixels];
    for (size_t i = 0; i < blockPixels; ++i)
        block[i] = __builtin_bswap16(color); // ensure MSB first

    uint32_t remaining = pixelCount;
    while (remaining) {
        size_t n = std::min(remaining, (uint32_t)blockPixels);
        mSpi.transferSync((uint8_t*)block, n * 16); // 16 bits per pixel
        remaining -= n;
    }
}

void Display::tick() {
    lv_timer_handler(); 

    static int x = 0;
    x = (x + 1) % 320;
    
    static float hue = 0.0f;
    hue = std::fmod(hue + 0.005f, 1.0f);
    Rgb565 rgb1= Rgb565::fromHsl(hue, 1.0f, 0.5f);
    testFillRect(x, 100, 1, 40, rgb1.value);

    Rgb565 rgb2 = Rgb565::fromFloat(ColorMaps::Test.interpolateRgb(hue));
    testFillRect(x, 150, 1, 40, rgb2.value);

    Rgb565 rgb3 = Rgb565::fromHsl(ColorMaps::Test.interpolateHsl(hue));
    testFillRect(x, 200, 1, 40, rgb3.value);

    if (mTestTimer.isExpired()) {
        mTestTimer.restart();
    }

}

void Display::handleFlush(const lv_area_t* area, uint8_t* pixelMap) {
    return;
    
    uint8_t cmd;
    uint8_t data[4];

    // set column address
    cmd = St7789vHelper::CASET;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
    mGpioDc.setValue(true);
    data[0] = ByteUtils::GetByte1(area->x1);
    data[1] = ByteUtils::GetByte0(area->x1);
    data[2] = ByteUtils::GetByte1(area->x2);
    data[3] = ByteUtils::GetByte0(area->x2);
    mSpi.transferSync(data, 32);

    // set row address
    cmd = St7789vHelper::RASET;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
    mGpioDc.setValue(true);
    data[0] = ByteUtils::GetByte1(area->y1);
    data[1] = ByteUtils::GetByte0(area->y1);
    data[2] = ByteUtils::GetByte1(area->y2);
    data[3] = ByteUtils::GetByte0(area->y2);
    mSpi.transferSync(data, 32);

    // memory write
    cmd = St7789vHelper::RAMWR;
    mGpioDc.setValue(false);
    mSpi.transferSync(&cmd, 8);
    mGpioDc.setValue(true);

    // send pixels
    uint32_t const width = area->x2 - area->x1 + 1;
    uint32_t const height = area->y2 - area->y1 + 1;
    uint32_t const sizeBytes = width * height * 2;
    mSpi.transferSync(pixelMap, sizeBytes, this);

    // notify lvgl
    lv_display_flush_ready(mLvDisplay);
}

void Display::handleTxComplete(bool success) {
    // nothing to do
}

uint32_t Display::tickCallback() {
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
            break;
        case LV_LOG_LEVEL_ERROR:
            LogError("LVGL/Error", "%s", str);
            break;
        case LV_LOG_LEVEL_USER:        
        default:
            AssertDebug(false, "Display::logCallback()", "unhandled log level");
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

}  // namespace Garbox
