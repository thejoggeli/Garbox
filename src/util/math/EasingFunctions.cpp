#include "EasingFunctions.h"
#include "util/math/MathConstants.h"

namespace Garbox {

float EasingFunctions::EaseLinear(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t;
}

float EasingFunctions::EaseInQuad(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * t;
}

float EasingFunctions::EaseOutQuad(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * (2.0f - t);
}

float EasingFunctions::EaseInOutQuad(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return (t < 0.5f) ? (2.0f * t * t) : (-1.0f + (4.0f - 2.0f * t) * t);
}

float EasingFunctions::EaseInSine(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return 1.0f - cosf((t * MathConstants::Pi) / 2.0f);
}

float EasingFunctions::EaseOutSine(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return sinf((t * MathConstants::Pi) / 2.0f);
}

float EasingFunctions::EaseInOutSine(float t) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return -(cosf(MathConstants::Pi * t) - 1.0f) / 2.0f;
}

float EasingFunctions::EaseInElastic(float t, float amplitude, float period) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float s = period / 4.0f;
    return -(amplitude * powf(2.0f, 10.0f * (t - 1.0f)) * sinf((t - 1.0f - s) * (2.0f * MathConstants::Pi) / period));
}

float EasingFunctions::EaseOutElastic(float t, float amplitude, float period) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float s = period / 4.0f;
    return (amplitude * powf(2.0f, -10.0f * t) * sinf((t - s) * (2.0f * MathConstants::Pi) / period) + 1.0f);
}

float EasingFunctions::EaseInOutElastic(float t, float amplitude, float period) {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float s = period / 4.0f;
    if (t < 0.5f) {
        return -0.5f * (amplitude * powf(2.0f, 20.0f * t - 10.0f) * sinf((t - s) * (2.0f * MathConstants::Pi) / period));
    } else {
        return amplitude * powf(2.0f, -20.0f * t + 10.0f) * sinf((t - s) * (2.0f * MathConstants::Pi) / period) * 0.5f + 1.0f;
    }
}

} // namespace Garbox
