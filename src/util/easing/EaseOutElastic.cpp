#include "EaseOutElastic.h"

#include <math.h>

EaseOutElastic::EaseOutElastic(float amplitude, float period) : mAmplitude(amplitude), mPeriod(period){
    // nothing to do
}

void EaseOutElastic::setAmplitude(float amplitude){
    mAmplitude = amplitude;
}

void EaseOutElastic::setPeriod(float period){
    mPeriod = period;
}

float EaseOutElastic::evaluate(float t) const {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float pi = 3.14159265f;
    const float s = mPeriod / 4.0f;
    return (mAmplitude * std::pow(2.0f, -10.0f * t) * sinf((t - s) * (2.0f * pi) / mPeriod) + 1.0f);
}
