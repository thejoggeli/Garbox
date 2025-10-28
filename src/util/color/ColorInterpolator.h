#pragma once

#include "RgbFloat.h"
#include "HslColor.h"

namespace Garbox {

class ColorInterpolator {
public:
    static RgbFloat interpolateRgb(const RgbFloat& a, const RgbFloat& b, float t);
    static HslColor interpolateHsl(const HslColor& a, const HslColor& b, float t);
};

} // namespace
