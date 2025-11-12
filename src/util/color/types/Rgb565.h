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

    static Rgb565 From(const HslColor& hsl);
    static Rgb565 From(const Rgb888& rgb);
    static Rgb565 From(const RgbFloat& rgb);

    static Rgb565 FromHsl(float h, float s, float l);
    static Rgb565 FromRgb888(uint8_t r, uint8_t g, uint8_t b);
    static Rgb565 FromRgbFloat(float r, float g, float b);
};

} // namespace