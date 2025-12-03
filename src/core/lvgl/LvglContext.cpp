#include "LvglContext.h"

#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <lvgl.h>
#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox {

LvglContext::LvglContext(lv_obj_t* root, uint32_t displayWidth, uint32_t displayHeight): 
    mRoot(root),
    mDisplayWidth(displayWidth),
    mDisplayHeight(displayHeight){
    LogDebug("LvglContext", "ctor w=%u, h=%u", displayWidth, displayHeight);
    AssertExit((root != nullptr), "LvglContext", "root is nullptr");
    AssertExit(displayWidth > 0, "LvglContext", "display width is 0");
    AssertExit(mDisplayHeight > 0, "LvglContext", "display height is 0");
}

void LvglContext::init(){
    LogDebug("LvglContext", "init");
    // disable scrollbar
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP);
    lv_obj_add_style(mRoot.raw(), &style, LV_PART_SCROLLBAR);
}

LvObject& LvglContext::root() { 
    return mRoot; 
} 

void LvglContext::getDisplaySize(uint32_t& width, uint32_t& height) const {
    width = mDisplayWidth;
    height = mDisplayHeight;
}

} // namespace Garbox
