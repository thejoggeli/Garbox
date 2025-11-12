#include "Rgb888.h"

#include "HslColor.h"
#include "RgbFloat.h"
#include "util/color/ColorConverter.h"

namespace Garbox {

Rgb888::Rgb888() : r(0), g(0), b(0) {}

Rgb888::Rgb888(uint8_t rVal, uint8_t gVal, uint8_t bVal) : r(rVal), g(gVal), b(bVal) {}

Rgb888 Rgb888::From(const Rgb565& rgb) {
    return ColorConverter::ToRgb888(rgb);
}

Rgb888 Rgb888::From(const RgbFloat& rgb) {
    return ColorConverter::ToRgb888(rgb);
}

Rgb888 Rgb888::From(const HslColor& hsl) {
    return ColorConverter::ToRgb888(hsl);
}
    
Rgb888 Rgb888::FromRgbFloat(float r, float g, float b){
    return ColorConverter::ToRgb888(RgbFloat(r, g, b));
}

Rgb888 Rgb888::FromHsl(float h, float s, float l) {
    return ColorConverter::ToRgb888(HslColor(h, s, l));
}

} // namespace