#pragma once

namespace Garbox {

class RgbFloat;

class LabColor {
public:
    float L;
    float a;
    float b;

    LabColor();
    LabColor(float L, float a, float b);

    RgbFloat toStandardRgb() const;
    RgbFloat toLinearRgb() const;

    static LabColor From(const RgbFloat& linearRgb);
};

} // namespace
