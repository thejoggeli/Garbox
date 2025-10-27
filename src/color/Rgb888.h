#pragma once

#include <cstdint>

namespace Garbox {

class RgbFloat;
class Rgb565;
class HslColor;

class Rgb888 {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Rgb888();
    Rgb888(uint8_t r, uint8_t g, uint8_t b);

    RgbFloat toFloat() const;
    Rgb565 toRgb565() const;
    HslColor toHsl() const;

    static Rgb888 fromFloat(const RgbFloat& f);
    static Rgb888 fromRgb565(const Rgb565& rgb);
    static Rgb888 fromHsl(const HslColor& hsl);
    static Rgb888 fromHsl(float h, float s, float l);
};

} // namespace
