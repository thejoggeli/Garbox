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

    RgbFloat toStandardRgb() const;
    RgbFloat toLinearRgb() const;

    uint32_t hex() const;

    static RgbFloat From(const HslColor& hsl); // returns RgbFloat in StandardRGB format
    static RgbFloat From(const LabColor& lab); // returns RgbFloat in LinearRGB format
    static RgbFloat From(const Rgb888& rgb);   // returns RgbFloat in StandardRGB format
    static RgbFloat From(const Rgb565& rgb);   // returns RgbFloat in StandardRGB format

    static RgbFloat FromHsl(float h, float s, float l);          // returns RgbFloat in StandardRGB format
    static RgbFloat FromLab(float L, float a, float b);          // returns RgbFloat in LinearRGB format
    static RgbFloat FromRgb888(uint8_t r, uint8_t g, uint8_t b); // returns RgbFloat in StandardRGB format
    static RgbFloat FromHex(uint32_t hex);

    // Operator overloads
    RgbFloat operator*(float scalar) const;
    RgbFloat operator/(float scalar) const;
    RgbFloat& operator*=(float scalar);
    RgbFloat& operator/=(float scalar);
};

} // namespace Garbox
