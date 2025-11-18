#pragma once

namespace Garbox {

class HslColor;
class LabColor;
class RgbFloat;

class ColorInterpolator {
public:
    static HslColor interpolateHsl(const HslColor& a, const HslColor& b, float t);
    static LabColor interpolateLab(const LabColor& a, const LabColor& b, float t);
    static RgbFloat interpolateRgb(const RgbFloat& a, const RgbFloat& b, float t);
};

} // namespace
