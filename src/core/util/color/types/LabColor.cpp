#include "LabColor.h"

#include "core/util/color/ColorConverter.h"
#include "core/util/color/types/HslColor.h"
#include "core/util/color/types/RgbFloat.h"

namespace Garbox {

LabColor::LabColor() : L(100.0f), a(0.0f), b(0.0f){
    // constructor body
}

LabColor::LabColor(float LVal, float aVal, float bVal) : L(LVal), a(aVal), b(bVal){
    // constructor body
}

uint32_t LabColor::hexStandard() const {
    return toStandardRgb().hex();
}

uint32_t LabColor::hexLinear() const {
    return toLinearRgb().hex();
}

HslColor LabColor::toHsl() const {
    return ColorConverter::ToHsl(*this);
}

RgbFloat LabColor::toStandardRgb() const {
    return ColorConverter::ToStandardRgb(*this);
}

RgbFloat LabColor::toLinearRgb() const {
    return ColorConverter::ToLinearRgb(*this);
}

LabColor LabColor::From(const HslColor& hsl){
    return ColorConverter::ToLab(hsl);
}

LabColor LabColor::From(const RgbFloat& linearRgb){
    return ColorConverter::ToLab(linearRgb);
}

} // namespace