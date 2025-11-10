#include "ExponentialFilter.h"

#include "assert/Assert.h"
#include <algorithm>
#include <cmath>

namespace Garbox {

ExponentialFilter::ExponentialFilter() : mOneMinusAlpha(1.0f - mAlpha){
    // nothing to do
}

ExponentialFilter::ExponentialFilter(float alpha, float threshold):
    // initialize members
    mThreshold(threshold),
    mAlpha(alpha),
    mOneMinusAlpha(1.0f - alpha){
    // constructor body
}

void ExponentialFilter::setAlpha(float alpha){
    mAlpha = std::clamp(alpha, 0.0f, 1.0f);
}

void ExponentialFilter::setAlphaComputed(float fraction, uint32_t ticks){
    mAlpha = computeAlpha(fraction, ticks);
}

void ExponentialFilter::setThreshold(float threshold){
    mThreshold = threshold;
}

float ExponentialFilter::getAlpha() const{
    return mAlpha;
}

float ExponentialFilter::getThreshold() const{
    return mThreshold;
}

float ExponentialFilter::onProcess(float value){
    // ignore minor changes (deadband)
    if(mThreshold != 0.0f && (std::fabs(value - mLastFiltered) < mThreshold)){
        return mLastFiltered;
    }

    // apply exponential smoothing
    return (mAlpha * value) + ((1.0f - mAlpha) * mLastFiltered);
}

float ExponentialFilter::computeAlpha(float fraction, uint32_t ticks){
    if(ticks == 0){
        return 1.0f;
    }
    if(fraction <= 0.0f){
        return 0.0f;
    }
    if(fraction >= 1.0f){
        return 1.0f;
    }

    const float remaining = 1.0f - fraction;
    return 1.0f - std::pow(remaining, 1.0f / static_cast<float>(ticks));
}

} // namespace Garbox
