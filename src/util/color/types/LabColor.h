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

    static LabColor From(const RgbFloat& linearRgb);
};

} // namespace
