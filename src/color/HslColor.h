#pragma once

namespace Garbox {

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

    RgbFloat toRgb() const;
    Rgb888 toRgb888() const;
    Rgb565 toRgb565() const;

    static HslColor fromRgb(const RgbFloat& rgb);
    static HslColor fromRgb888(const Rgb888& rgb);
    static HslColor fromRgb565(const Rgb565& rgb);
};

} // namespace
