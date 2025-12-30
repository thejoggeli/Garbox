#pragma once

#include <lvgl.h>

#include "LvObject.h"

namespace Garbox {

class LvArc : public LvObject {
public:
    explicit LvArc(LvObject& parent);

    void setArcValue(int32_t value);
    void setArcRange(int32_t min, int32_t max);
    void setArcRotation(uint16_t rotation);
    void setArcBgAngles(uint16_t start, uint16_t end);
    void setArcAngles(uint16_t start, uint16_t end);
    void setArcMode(lv_arc_mode_t mode);

    void setArcColor(lv_color_t color);
    void setArcWidth(int32_t width);
    void setArcRounded(bool enabled);
    void setArcOpa(lv_opa_t opa);
    void setArcImageSrc(const void* src);

    void setArcBgColor(lv_color_t color);
    void setArcBgWidth(int32_t width);
    void setArcBgRounded(bool enabled);
    void setArcBgOpa(lv_opa_t opa);
    void setArcBgImageSrc(const void* src);

    void removeKnob();

};

} // namespace Garbox
