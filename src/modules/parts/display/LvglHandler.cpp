#include "LvglHandler.h"

#include <cmath>
#include <esp_heap_caps.h>
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"

namespace Garbox {

LvglHandler::LvglHandler(const Config& config):
    mObjects(),
    mWidth(config.width),
    mHeight(config.height),
    mBytesPerPixel(config.bytesPerPixel),
    mPartialFactor(config.partialFactor),
    mBufferSize(mWidth * mHeight * mBytesPerPixel / mPartialFactor),
    mBufferWidth(mWidth),
    mBufferHeight(mHeight / mPartialFactor){
    // constructor body
    AssertExit(mWidth == mBufferWidth, "LvglHandler", "display and buffer width must be equal");
    AssertExit((mBufferHeight * mPartialFactor) == mHeight, "LvglHandler", "inconsistent buffer and display height");
    AssertExit((mWidth / mPartialFactor) > 0, "LvglHandler", "partial buffer height cannot be zero");
    AssertExit(mBytesPerPixel == 2, "LvglHandler", "RGB565 requires exactly 2 bytes per pixel");
    AssertExit(mBufferSize == (mWidth * mHeight * mBytesPerPixel / mPartialFactor), "LvglHandler", "frame size inconsistent with constants");
}

LvglHandler::~LvglHandler(){
    TriggerExit("LvglHandler", "std::function can use heap");
}

LvglObjects& LvglHandler::getObjects(){
    return mObjects;
}

void LvglHandler::init(){

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
    mLvDisplay = lv_display_create(mWidth, mHeight);
    lv_display_set_user_data(mLvDisplay, this); 

    // setup rendering
    lv_display_set_draw_buffers(mLvDisplay, &mDrawBuffer1, &mDrawBuffer2);
    lv_display_set_3rd_draw_buffer(mLvDisplay, &mDrawBuffer3);
    lv_display_set_render_mode(mLvDisplay, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // flush callbacks
    lv_display_set_flush_cb(mLvDisplay, handleFlush);
    lv_display_set_flush_wait_cb(mLvDisplay, handleFlushWait);

    // create background
    mBackground = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBackground, mWidth, mHeight);
    lv_obj_set_style_bg_color(mBackground, lv_color_hex(0x0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBackground, LV_OPA_COVER, LV_PART_MAIN);

    // init objects
    mObjects.init(lv_scr_act());
}

void LvglHandler::startRender(){    
    lv_timer_handler();
}

void LvglHandler::setFlushHandler(FlushHandler handler){
    mFlushHandler = handler;
}

void LvglHandler::setFlushWaitHandler(FlushWaitHandler handler){
    mFlushWaitHandler = handler;
}

void LvglHandler::handleFlush(lv_display_t* disp, const lv_area_t* area, uint8_t* pixelMap){
    AssertExit(disp != nullptr, "LvglHandler", "disp is nullptr");
    LvglHandler* self = static_cast<LvglHandler*>(lv_display_get_user_data(disp));
    if (self){
        AssertExit(self->mFlushHandler != nullptr, "LvglHandler", "flush handler nullptr");
        const RectXXYY rect {area->x1, area->x2, area->y1, area->y2};        
        self->mFlushHandler(rect, pixelMap);
    }
}

void LvglHandler::handleFlushWait(lv_display_t* disp){
    AssertExit(disp != nullptr, "LvglHandler", "disp is nullptr");
    LvglHandler* self = static_cast<LvglHandler*>(lv_display_get_user_data(disp));
    if (self){
        AssertExit(self->mFlushWaitHandler != nullptr, "LvglHandler", "flush wait handler nullptr");
        self->mFlushWaitHandler();
    }
}

void LvglHandler::handleLog(lv_log_level_t level, const char* str){
    switch(level){
    case LV_LOG_LEVEL_TRACE:
        LogDebug0("LVGL/Trace", "%s", str);
        break;
    case LV_LOG_LEVEL_INFO:
        LogInfo0("LVGL/Info", "%s", str);
        break;
    case LV_LOG_LEVEL_WARN:
        LogWarning0("LVGL/Warn", "%s", str);
        TriggerDebug("LvglHandler", "LVGL Warning");
        break;
    case LV_LOG_LEVEL_ERROR:
        LogError0("LVGL/Error", "%s", str);
        TriggerExit("LvglHandler", "LVGL Error");
        break;
    case LV_LOG_LEVEL_USER:        
    default:
        TriggerDebug("LvglHandler", "unhandled log level");
        break;
    }
}

uint32_t LvglHandler::lvglTickProvider(){
    return Time::GetMillis();
}

uint8_t* LvglHandler::allocDrawBufferData(uint32_t size){
    // alloc memory
    uint8_t* buffer = (uint8_t*) heap_caps_malloc(size, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    
    // check if alloc success
    AssertExit(buffer != nullptr, "LvglHandler", "mDrawBuffer is nullptr");
    if (!(heap_caps_get_allocated_size(buffer) && heap_caps_get_largest_free_block(MALLOC_CAP_DMA))){
        multi_heap_info_t info;
        heap_caps_get_info(&info, MALLOC_CAP_DMA);
        TriggerExit("LvglHandler", "DMA allocation failed", static_cast<int32_t>(info.total_free_bytes));
    }

    // pointer to buffer
    return buffer;
}

void LvglHandler::initDrawBuffer(lv_draw_buf_t& buffer, uint8_t* data, uint32_t size, uint32_t width, uint32_t height){
    buffer.header.cf = LV_COLOR_FORMAT_RGB565;      
    buffer.header.w = width;
    buffer.header.h = height;
    buffer.header.stride = width * sizeof(lv_color_t);
    buffer.data = data;
    buffer.data_size = size;
    buffer.unaligned_data = data;
    buffer.handlers = nullptr;
}

} // namespace