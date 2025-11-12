#pragma once

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

    RgbFloat toStandardRGB() const;
    RgbFloat toLinearRGB() const;

    static HslColor From(const RgbFloat& rgb);
    static HslColor From(const Rgb888& rgb);
    static HslColor From(const Rgb565& rgb);
};

} // namespace
