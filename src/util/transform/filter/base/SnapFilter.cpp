#include "SnapFilter.h"

#include "assert/Assert.h"
#include <cmath>
#include <algorithm>

namespace Garbox {

SnapFilter::SnapFilter(){
    // nothing to do
}

SnapFilter::SnapFilter(float resolution, float stickiness):
    // initialize members
    mResolution(resolution),
    mStickiness(stickiness){
    updateStickRange();
}

void SnapFilter::setResolution(float resolution){
    mResolution = (resolution > 0.0f) ? resolution : 0.0f;
    updateStickRange();
}

void SnapFilter::setStickiness(float stickiness){
    // clamp to [0, 1]
    mStickiness = std::clamp(stickiness, 0.0f, 1.0f);
    updateStickRange();
}

float SnapFilter::getResolution() const{
    return mResolution;
}

float SnapFilter::getStickiness() const{
    return mStickiness;
}

float SnapFilter::onProcess(float value){
    
    // snapping disabled
    if(mResolution <= 0.0f){
        return value;
    }

    const float snappedValue = snapValue(value);
    const float delta = std::fabs(snappedValue - mLastFiltered);

    if(delta <= mHalfStickRange){
        // within sticky band, hold last snapped value
        return mLastFiltered;
    }

    // move to new snapped value
    return mLastFiltered;
}

float SnapFilter::snapValue(float value) const{
    if(mResolution <= 0.0f){
        return value;
    }
    return std::round(value / mResolution) * mResolution;
}

void SnapFilter::updateStickRange(){
    mHalfStickRange = 0.5f * mResolution * mStickiness;
}

} // namespace Garbox
