#include "LabColor.h"

#include "util/color/ColorConverter.h"

namespace Garbox {

LabColor::LabColor() : L(100.0f), a(0.0f), b(0.0f) {
    // constructor body
}

LabColor::LabColor(float LVal, float aVal, float bVal) : L(LVal), a(aVal), b(bVal){
    // constructor body
}

LabColor LabColor::From(const RgbFloat& linearRgb) {
    return ColorConverter::ToLab(linearRgb);
}

} // namespace