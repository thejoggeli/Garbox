#include "EasingEquations.h"
#include "util/math/MathConstants.h"

namespace Garbox {

float EasingEquations::Linear(float t){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t;
}

float EasingEquations::InQuad(float t){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * t;
}

float EasingEquations::OutQuad(float t){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * (2.0f - t);
}

float EasingEquations::InOutQuad(float t){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return (t < 0.5f) ? (2.0f * t * t) : (-1.0f + (4.0f - 2.0f * t) * t);
}

float EasingEquations::InSine(float t){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return 1.0f - cosf((t * MathConstants::Pi) / 2.0f);
}

float EasingEquations::OutSine(float t){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return sinf((t * MathConstants::Pi) / 2.0f);
}

float EasingEquations::InOutSine(float t){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return -(cosf(MathConstants::Pi * t) - 1.0f) / 2.0f;
}

float EasingEquations::InElastic(float t, float amplitude, float period){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float s = period / 4.0f;
    return -(amplitude * powf(2.0f, 10.0f * (t - 1.0f)) * sinf((t - 1.0f - s) * (2.0f * MathConstants::Pi) / period));
}

float EasingEquations::OutElastic(float t, float amplitude, float period){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float s = period / 4.0f;
    return (amplitude * powf(2.0f, -10.0f * t) * sinf((t - s) * (2.0f * MathConstants::Pi) / period) + 1.0f);
}

float EasingEquations::InOutElastic(float t, float amplitude, float period){
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float s = period / 4.0f;
    if (t < 0.5f){
        return -0.5f * (amplitude * powf(2.0f, 20.0f * t - 10.0f) * sinf((t - s) * (2.0f * MathConstants::Pi) / period));
    } else {
        return amplitude * powf(2.0f, -20.0f * t + 10.0f) * sinf((t - s) * (2.0f * MathConstants::Pi) / period) * 0.5f + 1.0f;
    }
}

} // namespace Garbox
