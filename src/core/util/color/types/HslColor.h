#pragma once

#include <cstdint>

namespace Garbox {

class LabColor;
class RgbFloat;
class Rgb888;
class Rgb565;

class HslColor {
public:
    float h;
    float s;
    float l;

    HslColor();
    HslColor(float h, float s, float l);

    uint32_t hexStandard() const;
    uint32_t hexLinear() const;

    LabColor toLab() const;
    RgbFloat toStandardRgb() const;
    RgbFloat toLinearRgb() const;

    static HslColor From(const LabColor& rgb);
    static HslColor From(const RgbFloat& rgb);
    static HslColor From(const Rgb888& rgb);
    static HslColor From(const Rgb565& rgb);
};

} // namespace
