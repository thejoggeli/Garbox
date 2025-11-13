#include "RgbFloat.h"

#include "HslColor.h"
#include "LabColor.h"
#include "Rgb888.h"
#include "util/color/ColorConverter.h"
#include "util/function/default/GammaFunctions.h"

namespace Garbox {

RgbFloat::RgbFloat() : r(0.0f), g(0.0f), b(0.0f) {
    // constructor body
}

RgbFloat::RgbFloat(float rVal, float gVal, float bVal) : r(rVal), g(gVal), b(bVal) {
    // constructor body
}

RgbFloat RgbFloat::toStandardRgb() const {
    static const MathFunctionIfc& gammaInverse = GammaFunctions::GetGammaInverse22();
    return RgbFloat(
        gammaInverse.evaluate(r),
        gammaInverse.evaluate(g),
        gammaInverse.evaluate(b)
    );
}

RgbFloat RgbFloat::toLinearRgb() const {
    static const MathFunctionIfc& gamma = GammaFunctions::GetGamma22();
    return RgbFloat(
        gamma.evaluate(r),
        gamma.evaluate(g),
        gamma.evaluate(b)
    );
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
