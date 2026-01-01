#pragma once

#include <cstdint>

namespace Garbox {

class HslColor;
class RgbFloat;

class LabColor {
public:
    float L;
    float a;
    float b;

    LabColor();
    LabColor(float L, float a, float b);

    uint32_t hexStandard() const;
    uint32_t hexLinear() const;

    HslColor toHsl() const;
    RgbFloat toStandardRgb() const;
    RgbFloat toLinearRgb() const;

    static LabColor From(const HslColor& hsl); 
    static LabColor From(const RgbFloat& linearRgb); // expects RgbFloat to be in LinearRGB format
};

} // namespace
