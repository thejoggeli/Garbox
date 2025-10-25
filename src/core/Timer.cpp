#include "Timer.h"

#include "Time.h"

namespace Garbox {

Timer::Timer(){
    // nothing to do
}

void Timer::start(uint32_t durationMillis){
    mStartTimeMillis = Time::GetMillis();
    mDurationMillis = durationMillis;
}

void Timer::restart(){
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

void Timer::extend(uint32_t durationMillis){
    mDurationMillis += durationMillis;
}

bool Timer::isExpired(){
    if(mDurationMillis == 0){
        return true;
    }
    else if((Time::GetMillis() - mStartTimeMillis) > mDurationMillis){
        return true;
    }
    return false;
}


}