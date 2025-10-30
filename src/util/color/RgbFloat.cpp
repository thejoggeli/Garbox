#include "RgbFloat.h"

#include "Rgb888.h"
#include "Rgb565.h"
#include "HslColor.h"
#include "ColorConverter.h"

namespace Garbox {

RgbFloat::RgbFloat() : r(0.0f), g(0.0f), b(0.0f) {}

RgbFloat::RgbFloat(float rVal, float gVal, float bVal)
    : r(rVal), g(gVal), b(bVal) {}

HslColor RgbFloat::toHsl() const {
    return ColorConverter::rgbToHsl(*this);
}

Rgb888 RgbFloat::toRgb888() const {
    return Rgb888::fromFloat(*this);
}

Rgb565 RgbFloat::toRgb565() const {
    return Rgb565::fromFloat(*this);
}

RgbFloat RgbFloat::fromRgb888(const Rgb888& rgb) {
    return rgb.toFloat();
}

RgbFloat RgbFloat::fromRgb565(const Rgb565& rgb) {
    return rgb.toFloat();
}

RgbFloat RgbFloat::fromHsl(const HslColor& hsl) {
    return ColorConverter::hslToRgb(hsl);
}

// Operator overloads
RgbFloat RgbFloat::operator*(float scalar) const {
    return RgbFloat(r * scalar, g * scalar, b * scalar);
}

RgbFloat RgbFloat::operator/(float scalar) const {
    return RgbFloat(r / scalar, g / scalar, b / scalar);
}

RgbFloat& RgbFloat::operator*=(float scalar) {
    r *= scalar;
    g *= scalar;
    b *= scalar;
    return *this;
}

RgbFloat& RgbFloat::operator/=(float scalar) {
    r /= scalar;
    g /= scalar;
    b /= scalar;
    return *this;
}

} // namespace Garbox
