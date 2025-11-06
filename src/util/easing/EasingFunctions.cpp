#include "EasingFunctions.h"
#include "util/math/MathConstants.h"

namespace Garbox {

using ElasticParams = EasingFunctions::ElasticParams;

// -------------------- Simple easings --------------------

float EasingFunctions::EaseLinear(float t, const void* p) {
    (void)p;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t;
}

float EasingFunctions::EaseInQuad(float t, const void* p) {
    (void)p;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * t;
}

float EasingFunctions::EaseOutQuad(float t, const void* p) {
    (void)p;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * (2.0f - t);
}

float EasingFunctions::EaseInOutQuad(float t, const void* p) {
    (void)p;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return (t < 0.5f) ? (2.0f * t * t)
                      : (-1.0f + (4.0f - 2.0f * t) * t);
}

float EasingFunctions::EaseInSine(float t, const void* p) {
    (void)p;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return 1.0f - cosf((t * MathConstants::Pi) / 2.0f);
}

float EasingFunctions::EaseOutSine(float t, const void* p) {
    (void)p;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return sinf((t * MathConstants::Pi) / 2.0f);
}

float EasingFunctions::EaseInOutSine(float t, const void* p) {
    (void)p;
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return -(cosf(MathConstants::Pi * t) - 1.0f) / 2.0f;
}

// -------------------- Elastic easings --------------------

float EasingFunctions::EaseInElastic(float t, const void* p) {
    const ElasticParams* e = static_cast<const ElasticParams*>(p);
    const float amplitude = e ? e->amplitude : 1.0f;
    const float period = e ? e->period : 0.3f;

    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;

    const float s = period / 4.0f;
    return -(amplitude * powf(2.0f, 10.0f * (t - 1.0f)) *
             sinf((t - 1.0f - s) * (2.0f * MathConstants::Pi) / period));
}

float EasingFunctions::EaseOutElastic(float t, const void* p) {
    const ElasticParams* e = static_cast<const ElasticParams*>(p);
    const float amplitude = e ? e->amplitude : 1.0f;
    const float period = e ? e->period : 0.3f;

    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;

    const float s = period / 4.0f;
    return (amplitude * powf(2.0f, -10.0f * t) *
            sinf((t - s) * (2.0f * MathConstants::Pi) / period) + 1.0f);
}

float EasingFunctions::EaseInOutElastic(float t, const void* p) {
    const ElasticParams* e = static_cast<const ElasticParams*>(p);
    const float amplitude = e ? e->amplitude : 1.0f;
    const float period = e ? e->period : 0.3f;

    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;

    const float s = period / 4.0f;

    if (t < 0.5f) {
        return -0.5f * (amplitude * powf(2.0f, 20.0f * t - 10.0f) * sinf((t - s) * (2.0f * MathConstants::Pi) / period));
    } else {
        return amplitude * powf(2.0f, -20.0f * t + 10.0f) * sinf((t - s) * (2.0f * MathConstants::Pi) / period) * 0.5f + 1.0f;
    }
}

// -------------------- Parameter presets --------------------

const ElasticParams* EasingFunctions::GetElasticLightParams() {
    static ElasticParams p = {0.8f, 0.45f};
    return &p;
}

const ElasticParams* EasingFunctions::GetElasticMediumParams() {
    static ElasticParams p = {1.0f, 0.4f};
    return &p;
}

const ElasticParams* EasingFunctions::GetElasticStrongParams() {
    static ElasticParams p = {1.2f, 0.3f};
    return &p;
}

} // namespace Garbox
