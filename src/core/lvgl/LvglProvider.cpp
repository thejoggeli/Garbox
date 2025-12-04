#include "LvglProvider.h"

#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <lvgl.h>
#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox::LvglProvider {

static bool sInitialized = false;
static LvObject* sRoot = nullptr;
static uint32_t sDisplayWidth = 0;
static uint32_t sDisplayHeight = 0;

void Init(uint32_t displayWidth, uint32_t displayHeight){

    LogDebug("LvglProvider", "init w=%u, h=%u", displayWidth, displayHeight);
    AssertExit(!sInitialized, "LvglProvider", "already initialized");
    AssertExit(displayWidth > 0, "LvglProvider", "display width is 0");
    AssertExit(displayHeight > 0, "LvglProvider", "display height is 0");

    sRoot = new LvObject(lv_scr_act());
    sDisplayWidth = displayWidth;
    sDisplayHeight = displayHeight;

    // disable scrollbar
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP);
    lv_obj_add_style(sRoot->raw(), &style, LV_PART_SCROLLBAR);

    sInitialized = true;
}

LvObject& Root(){ 
    AssertExit(sInitialized, "LvglProvider", "not initialized");
    AssertExit(sRoot != nullptr, "LvglProvider", "root is nullptr");
    return *sRoot; 
} 

void GetDisplaySize(uint32_t& width, uint32_t& height){
    width = sDisplayWidth;
    height = sDisplayHeight;
}

uint32_t GetDisplayWidth(){
    return sDisplayWidth;
}

uint32_t GetDisplayHeight(){
    return sDisplayHeight;
}

} // namespace Garbox
