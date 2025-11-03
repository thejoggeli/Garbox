#pragma once

#include <cstdint>

namespace Garbox {

class RgbFloat;
class Rgb888;
class HslColor;

class Rgb565 {
public:
    uint16_t value;

    Rgb565();
    Rgb565(uint16_t val);
    Rgb565(uint8_t r5, uint8_t g6, uint8_t b5);

    RgbFloat toFloat() const;
    Rgb888 toRgb888() const;
    HslColor toHsl() const;

    static Rgb565 fromFloat(float r, float g, float b);
    static Rgb565 fromFloat(const RgbFloat& f);
    static Rgb565 fromRgb888(uint8_t r, uint8_t g, uint8_t b);
    static Rgb565 fromRgb888(const Rgb888& rgb);
    static Rgb565 fromHsl(float h, float s, float l);
    static Rgb565 fromHsl(const HslColor& hsl);
};

} // namespace