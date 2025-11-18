#pragma once

#include <cstdint>

namespace Garbox {

class HslColor;
class LabColor;
class RgbFloat;
class Rgb888;
class Rgb565;

class ColorConverter {
public:

    // any => Rgb565
    static Rgb565 ToRgb565(const HslColor& hsl);      // converts (HslColor => Rgb565) - Output in standard RGB (sRGB) space
    static Rgb565 ToRgb565(const Rgb888& rgb);        // converts (Rgb888   => Rgb565) - Output in will be in same color space as input
    static Rgb565 ToRgb565(const RgbFloat& rgb);      // converts (RgbFloat => Rgb565) - Output in will be in same color space as input

    // any => Rgb888
    static Rgb888 ToRgb888(const HslColor& hsl);      // converts (HslColor => Rgb888) - Output in standard RGB (sRGB) space
    static Rgb888 ToRgb888(const Rgb565& rgb);        // converts (Rgb565   => Rgb888) - Output in will be in same color space as input
    static Rgb888 ToRgb888(const RgbFloat& rgb);      // converts (RgbFloat => Rgb888) - Output in will be in same color space as input

    // any => RgbFloat
    static RgbFloat ToRgbFloat(const HslColor& hsl);  // converts (HslColor => RgbFloat) - Output in standard RGB (sRGB) space
    static RgbFloat ToRgbFloat(const Rgb888& rgb);    // converts (Rgb888   => RgbFloat) - Output in will be in same color space as input
    static RgbFloat ToRgbFloat(const Rgb565& rgb);    // converts (Rgb565   => RgbFloat) - Output in will be in same color space as input
    static RgbFloat ToRgbFloat(const LabColor& lab);  // converts (LabColor => RgbFloat) - Output in will be in linear RGB space

    // any => HslColor
    static HslColor ToHsl(const RgbFloat& sRgb); // converts (RgbFloat => HslColor) - Expects RGB to be in standard RGB (sRGB) space
    static HslColor ToHsl(const Rgb888& sRgb);   // converts (Rgb888   => HslColor) - Expects RGB to be in standard RGB (sRGB) space
    static HslColor ToHsl(const Rgb565& sRgb);   // converts (Rgb565   => HslColor) - Expects RGB to be in standard RGB (sRGB) space
    static HslColor ToHsl(const LabColor& lab);  // converts (LabColor => HslColor) - Output in HSL format

    // any => LabColor 
    static LabColor ToLab(const RgbFloat& linearRgb); // converts (RgbFloat => LabColor) - Expects RGB to be in linear RGB space 
    static LabColor ToLab(const HslColor& hsl);       // converts (RgbFloat => LabColor) - Input in HSL format

    // Standard RGB <=> Linear RGB
    static RgbFloat ToLinearRgb(const HslColor& hsl);   // converts (HslColor => RgbFloat)
    static RgbFloat ToLinearRgb(const LabColor& lab);   // converts (LabColor => RgbFloat)
    static RgbFloat ToLinearRgb(const RgbFloat& rgb);   // converts (RgbFloat => RgbFloat) - Expects input to be in standard RGB (sRGB) space
    static RgbFloat ToStandardRgb(const HslColor& hsl); // converts (HslColor => RgbFloat)
    static RgbFloat ToStandardRgb(const LabColor& lab); // converts (LabColor => RgbFloat)
    static RgbFloat ToStandardRgb(const RgbFloat& rgb); // converts (RgbFloat => RgbFloat) - Expects input to be in linear RGB space

};

} // namespace
