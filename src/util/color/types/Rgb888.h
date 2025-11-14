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
    Rgb888(uint32_t hex);
    Rgb888(uint8_t r, uint8_t g, uint8_t b);

    static Rgb888 From(const Rgb565& rgb);
    static Rgb888 From(const RgbFloat& rgb);
    static Rgb888 From(const HslColor& hsl);

    static Rgb888 FromRgbFloat(float r, float g, float b);
    static Rgb888 FromHsl(float h, float s, float l);
    static Rgb888 FromHex(uint32_t hex);
};

} // namespace
