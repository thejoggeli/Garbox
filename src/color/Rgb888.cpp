#include "Rgb888.h"

#include "Rgb565.h"
#include "RgbFloat.h"
#include "HslColor.h"
#include "ColorConverter.h"

namespace Garbox {

Rgb888::Rgb888() : r(0), g(0), b(0) {}

Rgb888::Rgb888(uint8_t rVal, uint8_t gVal, uint8_t bVal) : r(rVal), g(gVal), b(bVal) {}

RgbFloat Rgb888::toFloat() const {
    return RgbFloat(r / 255.0f, g / 255.0f, b / 255.0f);
}

Rgb888 Rgb888::fromFloat(const RgbFloat& f) {
    uint8_t r8 = static_cast<uint8_t>(f.r * 255.0f + 0.5f);
    uint8_t g8 = static_cast<uint8_t>(f.g * 255.0f + 0.5f);
    uint8_t b8 = static_cast<uint8_t>(f.b * 255.0f + 0.5f);
    return Rgb888(r8, g8, b8);
}

Rgb565 Rgb888::toRgb565() const {
    uint8_t r5 = r >> 3;
    uint8_t g6 = g >> 2;
    uint8_t b5 = b >> 3;
    return Rgb565(r5, g6, b5);
}

HslColor Rgb888::toHsl() const {
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;
    return ColorConverter::rgbToHsl(rf, gf, bf);
}

Rgb888 Rgb888::fromRgb565(const Rgb565& rgb) {
    return rgb.toRgb888();
}

Rgb888 Rgb888::fromHsl(const HslColor& hsl) {
    RgbFloat f = ColorConverter::hslToRgb(hsl);
    return Rgb888(
        static_cast<uint8_t>(f.r * 255.0f + 0.5f),
        static_cast<uint8_t>(f.g * 255.0f + 0.5f),
        static_cast<uint8_t>(f.b * 255.0f + 0.5f)
    );
}
Rgb888 Rgb888::fromHsl(float h, float s, float l) {
    HslColor hsl = HslColor(h, s, l);
    RgbFloat rgb = ColorConverter::hslToRgb(hsl);
    return Rgb888(
        static_cast<uint8_t>(rgb.r * 255.0f + 0.5f),
        static_cast<uint8_t>(rgb.g * 255.0f + 0.5f),
        static_cast<uint8_t>(rgb.b * 255.0f + 0.5f)
    );
}

} // namespace