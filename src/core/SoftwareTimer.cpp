#include "SoftwareTimer.h"

#include "Time.h"

namespace Garbox {

SoftwareTimer::SoftwareTimer(){
    // nothing to do
}

void SoftwareTimer::start(uint32_t durationMillis){
    mStartTimeMillis = Time::GetMillis();
    mDurationMillis = durationMillis;
}

void SoftwareTimer::reset(){
    mStartTimeMillis = 0;
    mDurationMillis = 0;
}

void SoftwareTimer::restart(){
    uint32_t currentTimeMillis = Time::GetMillis();

    // no duration set
    if (mDurationMillis == 0) {
        return;
    }

    // keep adding full periods until next target is in the future
    // works correctly with uint32_t wrap-around
    while ((uint32_t)(currentTimeMillis - mStartTimeMillis) >= mDurationMillis) {
        mStartTimeMillis += mDurationMillis;
    }
}

void SoftwareTimer::extend(uint32_t durationMillis){
    mDurationMillis += durationMillis;
}

bool SoftwareTimer::isExpired(){
    if(mDurationMillis == 0){
        return true;
    }
    else if((Time::GetMillis() - mStartTimeMillis) >= mDurationMillis){
        return true;
    }
    return false;
}

uint32_t SoftwareTimer::getElapsedMillis() const {
    return Time::GetMillis() - mStartTimeMillis;
}

}