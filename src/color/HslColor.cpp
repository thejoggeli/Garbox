#include "HslColor.h"

#include "RgbFloat.h"
#include "Rgb888.h"
#include "Rgb565.h"
#include "ColorConverter.h"

namespace Garbox {

HslColor::HslColor() : h(0.0f), s(0.0f), l(0.0f) {}

HslColor::HslColor(float hVal, float sVal, float lVal)
    : h(hVal), s(sVal), l(lVal) {}

HslColor HslColor::fromRgb(const RgbFloat& rgb) {
    return ColorConverter::rgbToHsl(rgb);
}

HslColor HslColor::fromRgb888(const Rgb888& rgb) {
    float rf = rgb.r / 255.0f;
    float gf = rgb.g / 255.0f;
    float bf = rgb.b / 255.0f;
    return ColorConverter::rgbToHsl(rf, gf, bf);
}

HslColor HslColor::fromRgb565(const Rgb565& rgb) {
    uint8_t r5 = (rgb.value >> 11) & 0x1F;
    uint8_t g6 = (rgb.value >> 5) & 0x3F;
    uint8_t b5 = rgb.value & 0x1F;
    float rf = r5 / 31.0f;
    float gf = g6 / 63.0f;
    float bf = b5 / 31.0f;
    return ColorConverter::rgbToHsl(rf, gf, bf);
}

RgbFloat HslColor::toRgb() const {
    return ColorConverter::hslToRgb(*this);
}

Rgb888 HslColor::toRgb888() const {
    RgbFloat f = ColorConverter::hslToRgb(*this);
    return Rgb888(
        static_cast<uint8_t>(f.r * 255.0f + 0.5f),
        static_cast<uint8_t>(f.g * 255.0f + 0.5f),
        static_cast<uint8_t>(f.b * 255.0f + 0.5f)
    );
}

Rgb565 HslColor::toRgb565() const {
    RgbFloat f = ColorConverter::hslToRgb(*this);
    uint8_t r5 = static_cast<uint8_t>(f.r * 31.0f + 0.5f);
    uint8_t g6 = static_cast<uint8_t>(f.g * 63.0f + 0.5f);
    uint8_t b5 = static_cast<uint8_t>(f.b * 31.0f + 0.5f);
    return Rgb565(r5, g6, b5);
}

} // namespace