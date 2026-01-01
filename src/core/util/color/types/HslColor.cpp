#include "HslColor.h"

#include "core/util/color/ColorConverter.h"
#include "core/util/color/types/LabColor.h"
#include "core/util/color/types/RgbFloat.h"

namespace Garbox {

HslColor::HslColor() : h(0.0f), s(0.0f), l(0.0f){
    // constructor body
}

HslColor::HslColor(float hVal, float sVal, float lVal) : h(hVal), s(sVal), l(lVal){
    // constructor body
}

uint32_t HslColor::hexStandard() const {
    return toStandardRgb().hex();
}

uint32_t HslColor::hexLinear() const {
    return toLinearRgb().hex();
}

LabColor HslColor::toLab() const {
    return ColorConverter::ToLab(*this);
}

RgbFloat HslColor::toStandardRgb() const {
    return ColorConverter::ToStandardRgb(*this);
}

RgbFloat HslColor::toLinearRgb() const {
    return ColorConverter::ToLinearRgb(*this);
}

HslColor HslColor::From(const RgbFloat& rgb){
    return ColorConverter::ToHsl(rgb);
}

HslColor HslColor::From(const Rgb888& rgb){
    return ColorConverter::ToHsl(rgb);
}

HslColor HslColor::From(const Rgb565& rgb){
    return ColorConverter::ToHsl(rgb);
}

HslColor HslColor::From(const LabColor& lab){
    return ColorConverter::ToHsl(lab);
}

} // namespace