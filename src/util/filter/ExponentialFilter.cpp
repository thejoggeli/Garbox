#include "ExponentialFilter.h"
#include <cmath>

namespace Garbox {

ExponentialFilter::ExponentialFilter(float alpha, float threshold)
    : mAlpha(clampAlpha(alpha)),
      mThreshold(threshold),
      mCurrentValue(0.0f)
{
}

ExponentialFilter::ExponentialFilter(float fraction, uint32_t ticks, float threshold)
    : mAlpha(computeAlpha(fraction, ticks)),
      mThreshold(threshold),
      mCurrentValue(0.0f)
{
}

void ExponentialFilter::setCurrentValue(float value) {
    mCurrentValue = value;
}

void ExponentialFilter::update(float targetValue) {
    if(mCurrentValue == targetValue){
        // nothing to do
    }
    else if (std::fabs(targetValue - mCurrentValue)){
        mCurrentValue = targetValue;
    }
    else {
        mCurrentValue = mAlpha * targetValue + (1.0f - mAlpha) * mCurrentValue;
    }
}

float ExponentialFilter::getCurrentValue() const {
    return mCurrentValue;
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
    return 1.0f - std::pow(remaining, 1.0f / static_cast<float>(ticks));
}

float ExponentialFilter::clampAlpha(float alpha) {
    if (alpha < 0.0f) return 0.0f;
    if (alpha > 1.0f) return 1.0f;
    return alpha;
}

} // namespace Garbox
