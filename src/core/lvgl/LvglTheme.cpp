#include "core/lvgl/LvglTheme.h"

#include <lvgl.h>
#include <themes/lv_theme_private.h>

#include "core/assert/Assert.h"

namespace Garbox::LvglTheme {

namespace {

void apply(lv_theme_t*, lv_obj_t* object){
    lv_obj_set_style_radius(object, 0, 0);
    lv_obj_set_style_border_width(object, 0, 0);
    lv_obj_set_style_pad_all(object, 0, 0);
    lv_obj_set_style_margin_all(object, 0, 0);
    lv_obj_set_style_bg_opa(object, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_row(object, 0, 0);
    lv_obj_set_style_pad_column(object, 0, 0);
    lv_obj_remove_flag(object, LV_OBJ_FLAG_SCROLLABLE);
}

} // namespace

void Install(lv_display_t* display){
    AssertExit(display != nullptr, "LvglTheme", "display is nullptr");
    if(!display){
        return;
    }

    lv_theme_t* base = lv_display_get_theme(display);
    AssertExit(base != nullptr, "LvglTheme", "base theme is nullptr");
    if(!base){
        return;
    }

    // LVGL themes must outlive every object using them. The application owns
    // one display on both target and host, so process-lifetime storage matches
    // the display lifetime contract.
    static lv_theme_t applicationTheme;
    applicationTheme = *base;
    lv_theme_set_parent(&applicationTheme, base);
    lv_theme_set_apply_cb(&applicationTheme, apply);
    lv_display_set_theme(display, &applicationTheme);
}

} // namespace Garbox::LvglTheme
