#pragma once

namespace Garbox {

class MathConstants {
public:

    // Prevent instantiation
    MathConstants() = delete;

    static constexpr float Pi           = 3.14159265358979323846f;
    static constexpr float TwoPi        = 6.28318530717958647692f;
    static constexpr float HalfPi       = 1.57079632679489661923f;
    static constexpr float QuarterPi    = 0.78539816339744830962f;

    static constexpr float DegToRad     = Pi / 180.0f;
    static constexpr float RadToDeg     = 180.0f / Pi;

    static constexpr float e            = 2.71828182845904523536f;
    static constexpr float Sqrt2        = 1.41421356237309504880f;
    static constexpr float InvSqrt2     = 0.70710678118654752440f;

};

} // namespace Garbox
