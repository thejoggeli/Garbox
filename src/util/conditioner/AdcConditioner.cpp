#include "AdcConditioner.h"
#include "assert/Assert.h"

namespace Garbox {

AdcConditioner::AdcConditioner(uint32_t windowSize, float fixedPointScaling):
    // initialize members
    mLinearFunction(),
    mMovingAverageFilter(windowSize, fixedPointScaling),
    mSnapFilter(),
    mChain{ &mLinearFunction, &mMovingAverageFilter, &mSnapFilter },
    mCompositeFilter(mChain, mResults, NumFilters){
    // nothing to do
}

void AdcConditioner::reset(){
    mCompositeFilter.reset(0.0f);
    mMovingAverageFilter.reset(0.0f);
    mSnapFilter.reset(0.0f);
}

void AdcConditioner::setCalibrationPoints(LinearFunction::Point p1, LinearFunction::Point p2){
    mLinearFunction.setPoints(p1, p2);
}

void AdcConditioner::setCalibrationPoints(float x1, float y1, float x2, float y2){
    mLinearFunction.setPoints(x1, y1, x2, y2);
}

void AdcConditioner::setFixedPointScaling(float scaleFactor){
    mMovingAverageFilter.setScaleFactor(scaleFactor);
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

float AdcConditioner::getUnfilteredValue() const {
    return mResults[0];
}

} // namespace Garbox
