#include "AdcConditioner.h"
#include "assert/Assert.h"

namespace Garbox {

AdcConditioner::AdcConditioner():
    // initialize members
    mLinearFunction(),
    mExponentialFilter(),
    mSnapFilter(),
    mChain{ &mLinearFunction, &mExponentialFilter, &mSnapFilter },
    mCompositeFilter(mChain, NumFilters){
    // nothing to do
}

void AdcConditioner::reset(){
    mCompositeFilter.reset(0.0f);
    mExponentialFilter.reset(0.0f);
    mSnapFilter.reset(0.0f);
}

void AdcConditioner::setInputThreshold(float threshold){
    mCompositeFilter.setInputThreshold(threshold);
}

void AdcConditioner::setCalibrationPoints(LinearFunction::Point p1, LinearFunction::Point p2){
    mLinearFunction.setPoints(p1, p2);
}

void AdcConditioner::setCalibrationPoints(float x1, float y1, float x2, float y2){
    mLinearFunction.setPoints(x1, y1, x2, y2);
}

void AdcConditioner::setAlpha(float alpha){
    mExponentialFilter.setAlpha(alpha);
}

void AdcConditioner::setAlphaComputed(float fraction, uint32_t ticks){
    mExponentialFilter.setAlphaComputed(fraction, ticks);
}

void AdcConditioner::setSnapping(float resolution, float stickiness){
    mSnapFilter.setResolution(resolution);
    mSnapFilter.setStickiness(stickiness);
}

float AdcConditioner::process(float inputValue){
    return mCompositeFilter.apply(inputValue);
}

float AdcConditioner::getFilteredValue() const {
    return mCompositeFilter.getFilteredValue();
}

float AdcConditioner::getRawValue() const {
    return mCompositeFilter.getRawValue();
}

} // namespace Garbox
