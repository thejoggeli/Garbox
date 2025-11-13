#include "LabColor.h"

#include "util/color/ColorConverter.h"
#include "util/color/types/RgbFloat.h"

namespace Garbox {

LabColor::LabColor() : L(100.0f), a(0.0f), b(0.0f) {
    // constructor body
}

LabColor::LabColor(float LVal, float aVal, float bVal) : L(LVal), a(aVal), b(bVal){
    // constructor body
}

RgbFloat LabColor::toStandardRgb() const {
    return ColorConverter::ToRgbFloat(*this).toStandardRgb();
}

RgbFloat LabColor::toLinearRgb() const {
    return ColorConverter::ToRgbFloat(*this); // already in linear space
}

LabColor LabColor::From(const RgbFloat& linearRgb) {
    return ColorConverter::ToLab(linearRgb);
}

} // namespace