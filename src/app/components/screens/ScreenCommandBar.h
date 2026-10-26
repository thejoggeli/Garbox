#pragma once

#include <cstdint>
#include "app/config/AppConfig.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/lvgl/objects/LvObject.h"

namespace Garbox {

inline constexpr int32_t ScreenCommandBarHeight = 20;
inline constexpr int32_t ScreenCommandBarTop =
    static_cast<int32_t>(AppConfig::DisplayHeight) - ScreenCommandBarHeight;

inline void ConfigureScreenCommandBar(
    LvObject& bar,
    LvLabel& pressHint,
    LvLabel& holdHint,
    const char* pressText,
    const char* holdText){

    bar.setPosition(0, ScreenCommandBarTop);
    bar.setRawSize(
        static_cast<int32_t>(AppConfig::DisplayWidth),
        ScreenCommandBarHeight
    );
    bar.setLayout(LV_LAYOUT_FLEX);
    bar.setFlexFlow(LV_FLEX_FLOW_ROW);
    bar.setFlexAlign(
        LV_FLEX_ALIGN_SPACE_BETWEEN,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );
    bar.setPad(8, 8, 0, 0);
    bar.setBgColor(lv_color_hex(0x0D141A));
    bar.setBgOpa(LV_OPA_COVER);
    bar.setBorder(1, lv_color_hex(0x26343D));
    bar.setBorderSide(LV_BORDER_SIDE_TOP);
    // The labels deliberately fill the complete command bar. Depending on
    // font rounding LVGL can otherwise decide that the bar overflows by one
    // pixel and draw a scrollbar over the right-hand hint.
    bar.setScrollable(false);
    lv_obj_set_scrollbar_mode(bar.raw(), LV_SCROLLBAR_MODE_OFF);

    pressHint.setWidth(LV_PCT(50));
    pressHint.setFont(&lv_font_unscii_8);
    pressHint.setTextColor(lv_color_hex(0x81909A));
    pressHint.setTextAlign(LV_TEXT_ALIGN_LEFT);
    pressHint.setText(pressText);

    holdHint.setWidth(LV_PCT(50));
    holdHint.setFont(&lv_font_unscii_8);
    holdHint.setTextColor(lv_color_hex(0x81909A));
    holdHint.setTextAlign(LV_TEXT_ALIGN_RIGHT);
    holdHint.setText(holdText);
}

} // namespace Garbox
