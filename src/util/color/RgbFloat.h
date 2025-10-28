#pragma once

namespace Garbox {

class Rgb888;
class Rgb565;
class HslColor;

class RgbFloat {
public:
    float r;
    float g;
    float b;

    RgbFloat();
    RgbFloat(float r, float g, float b);

    HslColor toHsl() const;
    Rgb888 toRgb888() const;
    Rgb565 toRgb565() const;

    static RgbFloat fromRgb888(const Rgb888& rgb);
    static RgbFloat fromRgb565(const Rgb565& rgb);
    static RgbFloat fromHsl(const HslColor& hsl);
};

} // namespace