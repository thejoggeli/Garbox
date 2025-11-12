#pragma once

#include <cstdint>

namespace Garbox {

class FunctionIfc;
class HslColor;
class LabColor;
class Rgb888;
class Rgb565;

class RgbFloat {
public:
    float r;
    float g;
    float b;

    RgbFloat();
    RgbFloat(float r, float g, float b);

    RgbFloat toStandardRGB() const;
    RgbFloat toLinearRGB() const;

    static RgbFloat From(const HslColor& hsl);
    static RgbFloat From(const LabColor& lab);
    static RgbFloat From(const Rgb888& rgb);
    static RgbFloat From(const Rgb565& rgb);

    static RgbFloat FromHsl(float h, float s, float l);
    static RgbFloat FromLab(float L, float a, float b);
    static RgbFloat FromRgb888(uint8_t r, uint8_t g, uint8_t b);

    // Operator overloads
    RgbFloat operator*(float scalar) const;
    RgbFloat operator/(float scalar) const;
    RgbFloat& operator*=(float scalar);
    RgbFloat& operator/=(float scalar);
};

} // namespace Garbox
