#include "TachoConditioner.h"
#include "core/assert/Assert.h"

namespace Garbox {

TachoConditioner::TachoConditioner(uint32_t windowSize, float fixedPointScaleFactor):
    // initialize members
    mScaleFunction(),
    mMovingAverageFilter(windowSize, fixedPointScaleFactor),
    mSnapFilter(),
    mChain{ &mScaleFunction, &mMovingAverageFilter, &mSnapFilter },
    mCompositeFilter(mChain, mResults, NumFilters){
    // nothing to do
}

void TachoConditioner::reset(){
    mCompositeFilter.reset(0.0f);
    mScaleFunction.setScale(1.0f);
    mMovingAverageFilter.reset(0.0f);
    mSnapFilter.reset(0.0f);
}

void TachoConditioner::setInputScaling(float scaling){
    mScaleFunction.setScale(scaling);
}

void TachoConditioner::setFixedPointScaling(float scaleFactor){
    mMovingAverageFilter.setScaleFactor(scaleFactor);
}

void TachoConditioner::setOutputSnapping(float resolution, float stickiness){
    mSnapFilter.setResolution(resolution);
    mSnapFilter.setStickiness(stickiness);
}

float TachoConditioner::process(float value){
    return mCompositeFilter.apply(value);
}

float TachoConditioner::getFilteredValue() const {
    return mCompositeFilter.getFilteredValue();
}

float TachoConditioner::getRawValue() const {
    return mCompositeFilter.getRawValue();
}

float TachoConditioner::getUnfilteredValue() const {
    return mResults[0];
}

} // namespace Garbox
