#pragma once

namespace Garbox {

class RgbFloat;
class HslColor;

class ColorConverter {
public:
    static HslColor rgbToHsl(float r, float g, float b);
    static HslColor rgbToHsl(const RgbFloat& rgb);
    static RgbFloat hslToRgb(const HslColor& hsl);
};

} // namespace