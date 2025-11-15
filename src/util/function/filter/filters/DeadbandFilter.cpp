#include "DeadbandFilter.h"

#include "assert/Assert.h"
#include <cmath>

namespace Garbox {

DeadbandFilter::DeadbandFilter(){
    // nothing to do
}

DeadbandFilter::DeadbandFilter(float threshold):
    // initialize members
    mThreshold(threshold){
    // constructor body
}

void DeadbandFilter::setThreshold(float threshold){
    mThreshold = std::max(threshold, 0.0f);
}

float DeadbandFilter::getThreshold() const {
    return mThreshold;
}

float DeadbandFilter::onProcess(float value){
    const float delta = std::fabs(value - mLastFiltered);

    if(delta < mThreshold){
        // within deadband, hold previous output
        return mLastFiltered;
    }

    // outside deadband, accept new value
    return value;
}

} // namespace Garbox
