#pragma once

#include <lvgl.h>
#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvImage.h"

namespace Garbox {

class LvglHelpers {
public:

    enum class Angle : uint16_t {
        Deg0   = 0,
        Deg90  = 90,
        Deg180 = 180,
        Deg270 = 270
    };

    struct RotatedLabelConfig {
        const char* text;
        const lv_font_t* font;
        uint32_t color;
        uint32_t bgColor;
        Angle angle;
        int32_t letterSpace = 0;
        int32_t lineSpace = 0;
    };

    static LvImage createRotatedLabelRgb565(LvObject& parent, const RotatedLabelConfig& config);

};

} // namespace Garbox
