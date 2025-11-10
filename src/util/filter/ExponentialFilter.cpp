#include "ExponentialFilter.h"

#include <algorithm>
#include <cmath>

namespace Garbox {

ExponentialFilter::ExponentialFilter(){
    // constructor body
}

ExponentialFilter::ExponentialFilter(float alpha, float threshold): 
    // init members
    mAlpha(alpha),
    mThreshold(threshold){
    // constructor body
}

void ExponentialFilter::update(float targetValue) {
    const float delta = targetValue - mFilteredValue;

    if(std::fabs(delta) < mThreshold){
        // change too small, ignore noise
        return;
    }

    // exponential smoothing
    mFilteredValue += mAlpha * delta;
}

void ExponentialFilter::setFilteredValue(float value) {
    mFilteredValue = value;
}

float ExponentialFilter::getFilteredValue() const {
    return mFilteredValue;
}

float ExponentialFilter::getAlpha() const {
    return mAlpha;
}

float ExponentialFilter::getThreshold() const {
    return mThreshold;
}

void ExponentialFilter::setAlpha(float alpha) {
    mAlpha = clampAlpha(alpha);
}

void ExponentialFilter::setAlphaComputed(float fraction, uint32_t ticks) {
    mAlpha = computeAlpha(fraction, ticks);
}

void ExponentialFilter::setThreshold(float threshold) {
    mThreshold = threshold;
}

float ExponentialFilter::computeAlpha(float fraction, uint32_t ticks) {
    if (ticks == 0)
        return 1.0f;
    if (fraction <= 0.0f)
        return 0.0f;
    if (fraction >= 1.0f)
        return 1.0f;

    float remaining = 1.0f - fraction;
    float alpha = 1.0f - std::pow(remaining, 1.0f / static_cast<float>(ticks));
    return std::clamp(alpha, 0.0f, 1.0f);
}

float ExponentialFilter::clampAlpha(float alpha) {
    if (alpha < 0.0f) return 0.0f;
    if (alpha > 1.0f) return 1.0f;
    return alpha;
}

} // namespace Garbox
