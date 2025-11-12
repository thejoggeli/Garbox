#include "HslColor.h"

#include "util/color/ColorConverter.h"

namespace Garbox {

HslColor::HslColor() : h(0.0f), s(0.0f), l(0.0f) {
    // constructor body
}

HslColor::HslColor(float hVal, float sVal, float lVal) : h(hVal), s(sVal), l(lVal){
    // constructor body
}

HslColor HslColor::From(const RgbFloat& rgb) {
    return ColorConverter::ToHsl(rgb);
}

HslColor HslColor::From(const Rgb888& rgb) {
    return ColorConverter::ToHsl(rgb);
}

HslColor HslColor::From(const Rgb565& rgb) {
    return ColorConverter::ToHsl(rgb);
}

} // namespace