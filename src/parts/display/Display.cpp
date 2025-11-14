#include "Display.h"

#include <cmath>
#include "assert/Assert.h"
#include "core/log/Log.h"
#include "core/hardware/bus/SpiDma.h"
#include "core/time/Time.h"
#include "esp_heap_caps.h"
#include "global/providers/ColorMaps.h"
#include "util/ByteUtils.h"
#include "util/color/types/Rgb565.h"

namespace Garbox {

#define GarboxDebugDisplay 1

Display::Display(SpiDma& spi, Gpio& gpioRst, Gpio& gpioDc, LedcChannel& pwmBlk): 
    // init members 
    mSpi(spi),
    mSt7789v(gpioRst, gpioDc, pwmBlk),
    mWidth(AppConfig::DisplayWidth),
    mHeight(AppConfig::DisplayHeight),
    mPartialFactor(AppConfig::DisplayPartialFactor),
    mBytesPerPixel(AppConfig::DisplayBytesPerPixel),
    mBufferSize(AppConfig::DisplayBytesPerFlush),
    mBufferWidth(AppConfig::DisplayWidth),
    mBufferHeight(AppConfig::DisplayHeight / AppConfig::DisplayPartialFactor){

    // constructor body
    AssertExit(mWidth == mBufferWidth, "Display", "display and buffer width must be equal");
    AssertExit((mBufferHeight * mPartialFactor) == mHeight, "Display", "inconsistent buffer and display height");
    AssertExit((mWidth / mPartialFactor) > 0, "Display", "partial buffer height cannot be zero");
    AssertExit(mBytesPerPixel == 2, "Display", "RGB565 requires exactly 2 bytes per pixel");
    AssertExit(mBufferSize == (mWidth * mHeight * mBytesPerPixel / mPartialFactor), "Display", "frame size inconsistent with constants");
}

void Display::init() {

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
    initDrawBuffer(mDrawBuffer2, mDrawBufferData1, mBufferSize, mBufferWidth, mBufferHeight);
    initDrawBuffer(mDrawBuffer3, mDrawBufferData1, mBufferSize, mBufferWidth, mBufferHeight);

    // init display
    mLvDisplay = lv_display_create(mSt7789v.getWidth(), mSt7789v.getHeight());
    lv_display_set_user_data(mLvDisplay, this); 

    // setup rendering
    lv_display_set_draw_buffers(mLvDisplay, &mDrawBuffer1, &mDrawBuffer2);
    lv_display_set_3rd_draw_buffer(mLvDisplay, &mDrawBuffer3);
    lv_display_set_render_mode(mLvDisplay, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // flush callbacks
    lv_display_set_flush_cb(mLvDisplay, flushTrampoline);
    // lv_display_set_flush_wait_cb(mLvDisplay, flushWaitTrampoline);

    // create label
    mLabel = lv_label_create(lv_screen_active());
    lv_label_set_text(mLabel, "Garbox Display");
    lv_obj_set_style_text_font(mLabel, &lv_font_unscii_16, LV_PART_MAIN);
    lv_obj_set_align(mLabel, lv_align_t::LV_ALIGN_TOP_MID);

    // send initial commands to display
    const Rgb565 clearColor = Rgb565::FromRgbFloat(0, 0, 0);
    mSt7789v.sendFillColor(clearColor.value);

    // initialization complete
    mSt7789v.setBrightness(0.75f);
    mTestTimer.start(2000_ms);
    mInitialized = true;
}

void Display::tick() {

    const uint32_t millis = static_cast<float>(Time::GetMillis());
    lv_label_set_text_fmt(mLabel, "%04u.%03u", millis/1000, millis%1000);

    lv_timer_handler(); 

    // advance x
    static int x = 0;
    x = (x + 1) % 320;
    
    // advance t
    static float t = 0.0f;
    t = std::fmod(t + 0.002f, 1.0f);

    // get a colormap for interpolation
    static const ColorMap& colorMap = ColorMaps::GetTestDisplay();

    // draw rectangle with hsl rainbow
    Rgb565 rgb1 = Rgb565::FromHsl(t, 1.0f, 0.5f);
    mSt7789v.sendFillRect(x, 40, 1, 30, rgb1.value);

    // draw rectangle with colormap (rgb interpolation)
    Rgb565 rgb2 = Rgb565::From(colorMap.interpolateStandardRgb(t));
    mSt7789v.sendFillRect(x, 80, 1, 30, rgb2.value);

    // draw rectangle with colormap (rgb interpolation)
    Rgb565 rgb3 = Rgb565::From(colorMap.interpolateLinearRgb(t).toStandardRgb());
    mSt7789v.sendFillRect(x, 120, 1, 30, rgb3.value);

    // draw rectangle with colormap (hsl interpolation)
    Rgb565 rgb4 = Rgb565::From(colorMap.interpolateHsl(t));
    mSt7789v.sendFillRect(x, 160, 1, 30, rgb4.value);

    // draw rectangle with colormap (hsl interpolation)
    Rgb565 rgb5 = Rgb565::From(colorMap.interpolateLab(t).toStandardRgb());
    mSt7789v.sendFillRect(x, 200, 1, 30, rgb5.value);

    // test timer
    if (mTestTimer.isExpired()) {
        mTestTimer.restart();
    }
}

void Display::handleFlush(const lv_area_t* area, uint8_t* pixelMap){

#if GarboxDebugDisplay
    if(pixelMap == mDrawBufferData1){
        LogDebug("Display", "buffer1");
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
    const uint32_t width = area->x2 - area->x1 + 1;
    const uint32_t height = area->y2 - area->y1 + 1;
    const uint32_t sizeBytes = width * height * 2;
    const bool async = false;

    mSt7789v.sendDrawBufferXXYY(area->x1, area->x2, area->y1, area->y2, pixelMap, sizeBytes, async);
    
    // notify lvgl that transfer is complete
    lv_display_flush_ready(mLvDisplay);
}

void Display::handleFlushWait(){
    // TODO
}

void Display::handleSt7789vSendSync(const uint8_t* data, size_t numBytes){
    mSpi.transferSync(data, numBytes*8);
    // LogDebug("Display", "sending %u bytes (sync)", numBytes);
}

void Display::handleSt7789vSendAsync(const uint8_t* data, size_t numBytes){
    mSpi.transferSync(data, numBytes*8);
    // LogDebug("Display", "sending %u bytes (async)", numBytes);
}

void Display::handleTxComplete(bool success) {
    // nothing to do
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

void Display::flushWaitTrampoline(lv_display_t* disp){
    Display* self = static_cast<Display*>(lv_display_get_user_data(disp));
    if (self){
        self->handleFlushWait();
    }
}

void Display::txCompleteTrampoline(void* user, bool success) {
    static_cast<Display*>(user)->handleTxComplete(success);
}

uint32_t Display::lvglTickProvider() {
    return Time::GetMillis();
}

uint8_t* Display::allocDrawBufferData(uint32_t size){

    // alloc memory
    uint8_t* buffer = (uint8_t*) heap_caps_malloc(size, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    
    // check if alloc success
    AssertExit(buffer != nullptr, "Display", "mDrawBuffer is nullptr");
    if (!(heap_caps_get_allocated_size(buffer) && heap_caps_get_largest_free_block(MALLOC_CAP_DMA))) {
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
