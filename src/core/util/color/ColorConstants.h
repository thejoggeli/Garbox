#pragma once

#include <cstdint>

namespace Garbox {

class ColorConstants {
public:

    // D65 white point (CIE 1931 2 degree)
    static constexpr float LabD65Xn = 95.047f;
    static constexpr float LabD65Yn = 100.000f;
    static constexpr float LabD65Zn = 108.883f;

    // CIE Lab constants
    static constexpr float LabEpsilon = 216.0f / 24389.0f; // approx 0.008856
    static constexpr float LabKappa   = 24389.0f / 27.0f;  // approx 903.3
};

} // namespace
