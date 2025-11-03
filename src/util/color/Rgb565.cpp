#include "Rgb565.h"

#include "Rgb888.h"
#include "RgbFloat.h"
#include "HslColor.h"
#include "ColorConverter.h"

namespace Garbox {

Rgb565::Rgb565() : value(0) {}

Rgb565::Rgb565(uint16_t val) : value(val) {}

Rgb565::Rgb565(uint8_t r5, uint8_t g6, uint8_t b5) {
    value = ((r5 & 0x1F) << 11) | ((g6 & 0x3F) << 5) | (b5 & 0x1F);
}

RgbFloat Rgb565::toFloat() const {
    uint8_t r5 = (value >> 11) & 0x1F;
    uint8_t g6 = (value >> 5) & 0x3F;
    uint8_t b5 = value & 0x1F;
    return RgbFloat(r5 / 31.0f, g6 / 63.0f, b5 / 31.0f);
}

Rgb565 Rgb565::fromFloat(float r, float g, float b) {
    return Rgb565::fromFloat(RgbFloat(r, g, b));
}

Rgb565 Rgb565::fromFloat(const RgbFloat& f) {
    uint8_t r5 = static_cast<uint8_t>(f.r * 31.0f + 0.5f);
    uint8_t g6 = static_cast<uint8_t>(f.g * 63.0f + 0.5f);
    uint8_t b5 = static_cast<uint8_t>(f.b * 31.0f + 0.5f);
    return Rgb565(r5, g6, b5);
}

Rgb888 Rgb565::toRgb888() const {
    uint8_t r5 = (value >> 11) & 0x1F;
    uint8_t g6 = (value >> 5) & 0x3F;
    uint8_t b5 = value & 0x1F;

    uint8_t r8 = (r5 << 3) | (r5 >> 2);
    uint8_t g8 = (g6 << 2) | (g6 >> 4);
    uint8_t b8 = (b5 << 3) | (b5 >> 2);

    return Rgb888(r8, g8, b8);
}

HslColor Rgb565::toHsl() const {
    uint8_t r5 = (value >> 11) & 0x1F;
    uint8_t g6 = (value >> 5) & 0x3F;
    uint8_t b5 = value & 0x1F;

    float rf = r5 / 31.0f;
    float gf = g6 / 63.0f;
    float bf = b5 / 31.0f;

    return ColorConverter::rgbToHsl(rf, gf, bf);
}

Rgb565 Rgb565::fromRgb888(uint8_t r, uint8_t g, uint8_t b){
    return Rgb565::fromRgb888(Rgb888(r, g, b));
}

Rgb565 Rgb565::fromRgb888(const Rgb888& rgb) {
    uint8_t r5 = rgb.r >> 3;
    uint8_t g6 = rgb.g >> 2;
    uint8_t b5 = rgb.b >> 3;
    return Rgb565(r5, g6, b5);
}

Rgb565 Rgb565::fromHsl(float h, float s, float l){
    return Rgb565::fromHsl(HslColor(h, s, l));
}

Rgb565 Rgb565::fromHsl(const HslColor& hsl) {
    RgbFloat f = ColorConverter::hslToRgb(hsl);
    uint8_t r5 = static_cast<uint8_t>(f.r * 31.0f + 0.5f);
    uint8_t g6 = static_cast<uint8_t>(f.g * 63.0f + 0.5f);
    uint8_t b5 = static_cast<uint8_t>(f.b * 31.0f + 0.5f);
    return Rgb565(r5, g6, b5);
}

} // namespace 