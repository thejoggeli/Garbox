#include "LvglContext.h"

#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <lvgl.h>
#include "core/assert/Assert.h"

namespace Garbox {

LvglContext::LvglContext(lv_obj_t* root) : mRoot(root){
    AssertExit((root != nullptr), "LvglContext", "root is nullptr");
}

// helper to set formatted text on a label
void LvglContext::setFormatted(lv_obj_t* lbl, const char* fmt, ...) {
    char buffer[64];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    lv_label_set_text(lbl, buffer);
}

void LvglContext::init(){
    
    // disable scrollbar
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP);
    lv_obj_add_style(mRoot.raw(), &style, LV_PART_SCROLLBAR);

    // create background
    mBackground = lv_obj_create(mRoot.raw());
    lv_obj_set_size(mBackground, 320, 240);
    lv_obj_set_style_bg_color(mBackground, lv_color_hex(0x0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBackground, LV_OPA_COVER, LV_PART_MAIN);

    // create box1
    mBox = lv_obj_create(mRoot.raw());
    lv_obj_set_size(mBox, 48, 8);
    lv_obj_set_style_bg_color(mBox, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBox, LV_OPA_COVER, LV_PART_MAIN);
}

void LvglContext::setBoxPosition(float position){
    static constexpr uint32_t y = 240-8;
    static constexpr float wDisplay = 320.0f; 
    static constexpr float wBox = 48.0f;
    static constexpr float left = -wBox;
    static constexpr float right = wDisplay;
    const float x = std::clamp(position * (right - left) + left, left, right);
    lv_obj_set_pos(mBox, x, y);
}

void LvglContext::setBackgroundColor(uint32_t color){
    lv_obj_set_style_bg_color(mBackground, lv_color_hex(color), LV_PART_MAIN);
}

LvObject& LvglContext::getRoot() { 
    return mRoot; 
} 

} // namespace Garbox
