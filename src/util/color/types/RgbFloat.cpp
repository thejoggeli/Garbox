#include "RgbFloat.h"

#include "HslColor.h"
#include "LabColor.h"
#include "Rgb888.h"
#include "util/color/ColorConverter.h"

namespace Garbox {

RgbFloat::RgbFloat() : r(0.0f), g(0.0f), b(0.0f){
    // constructor body
}

RgbFloat::RgbFloat(float rVal, float gVal, float bVal) : r(rVal), g(gVal), b(bVal){
    // constructor body
}

RgbFloat RgbFloat::toStandardRgb() const {
    return ColorConverter::ToStandardRgb(*this);
}

RgbFloat RgbFloat::toLinearRgb() const {
    return ColorConverter::ToLinearRgb(*this);
}

RgbFloat RgbFloat::From(const HslColor& hsl){
    return ColorConverter::ToRgbFloat(hsl);
}

RgbFloat RgbFloat::From(const LabColor& lab){
    return ColorConverter::ToRgbFloat(lab);
}

RgbFloat RgbFloat::From(const Rgb888& rgb){
    return ColorConverter::ToRgbFloat(rgb);
}

RgbFloat RgbFloat::From(const Rgb565& rgb){
    return ColorConverter::ToRgbFloat(rgb);    
}

RgbFloat RgbFloat::FromHsl(float h, float s, float l){
    return ColorConverter::ToRgbFloat(HslColor(h, s, l));
}

RgbFloat RgbFloat::FromLab(float L, float a, float b){
    return ColorConverter::ToRgbFloat(LabColor(L, a, b));
}

RgbFloat RgbFloat::FromRgb888(uint8_t r, uint8_t g, uint8_t b){
    return ColorConverter::ToRgbFloat(Rgb888(r, g, b));
}

RgbFloat RgbFloat::FromHex(uint32_t hex){
    return RgbFloat::From(Rgb888(hex));
}

// Operator overloads
RgbFloat RgbFloat::operator*(float scalar) const {
    return RgbFloat(r * scalar, g * scalar, b * scalar);
}

RgbFloat RgbFloat::operator/(float scalar) const {
    return RgbFloat(r / scalar, g / scalar, b / scalar);
}

RgbFloat& RgbFloat::operator*=(float scalar){
    r *= scalar;
    g *= scalar;
    b *= scalar;
    return *this;
}

RgbFloat& RgbFloat::operator/=(float scalar){
    r /= scalar;
    g /= scalar;
    b /= scalar;
    return *this;
}

} // namespace Garbox
