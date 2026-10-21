#include "SoftwareTimer.h"

#include "Time.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

SoftwareTimer::SoftwareTimer(){
    // nothing to do
}

void SoftwareTimer::start(uint32_t durationMicros){
    mStartTimeMicros = Time::GetMicros();
    mDurationMicros = durationMicros;
    mState = State::Running;
}

void SoftwareTimer::reset(){
    mStartTimeMicros = 0;
    mDurationMicros = 0;
    mState = State::Reset;
}

void SoftwareTimer::restart(){
    if(mState == State::Reset){
        // start and expired immediately
        start(0);
        return;
    }
    restartInner();
}

void SoftwareTimer::restart(uint32_t newDurationMicros){
    if(mState == State::Reset){
        // start with new duration
        start(newDurationMicros);
        return;
    }
    restartInner();
    mDurationMicros = newDurationMicros;
}

void SoftwareTimer::restartInner(){
    // reset start time only
    if (mDurationMicros == 0){
        mStartTimeMicros = Time::GetMicros();
        return;
    }

    // get current time
    uint32_t currentTimeMicros = Time::GetMicros();

    // Advance directly to the most recent period boundary. Unsigned
    // subtraction/multiplication preserve the intended modulo-2^32 timer
    // behavior without an unbounded catch-up loop after a long stall.
    const uint32_t elapsedMicros = currentTimeMicros - mStartTimeMicros;
    if(elapsedMicros >= mDurationMicros){
        const uint32_t elapsedPeriods = elapsedMicros / mDurationMicros;
        mStartTimeMicros += elapsedPeriods * mDurationMicros;
    }
}

void SoftwareTimer::extend(uint32_t durationMicros){
    // cannot exted in reset state
    if(mState == State::Reset){
        return;
    }
    mDurationMicros += durationMicros;
}

bool SoftwareTimer::isReset() const {
    return mState == State::Reset;
}

bool SoftwareTimer::isRunning() const {
    return mState == State::Running;
}

bool SoftwareTimer::isExpired() const {
    if(mState == State::Reset){
        return false; // timer is not running
    }
    if(mDurationMicros == 0){
        return true; // timer is expired
    }
    if((uint32_t)(Time::GetMicros() - mStartTimeMicros) >= mDurationMicros){
        return true; // timer is expired
    }
    return false; // timer is not expired
}

bool SoftwareTimer::isRunningAndNotExpired() const {
    if(mState == State::Reset){
        return false; // timer is not running
    }
    if(mDurationMicros == 0){
        return false; // timer is expired
    }
    if((uint32_t)(Time::GetMicros() - mStartTimeMicros) >= mDurationMicros){
        return false; // timer is expired
    }
    return true; // timer is not expired
    
}

uint32_t SoftwareTimer::getElapsedMicros() const {
    if(mState == State::Reset){
        return 0;
    }
    return Time::GetMicros() - mStartTimeMicros;
}

float SoftwareTimer::getElapsedFraction(bool clamp) const {
    if(mState == State::Reset){
        return 0.0f;
    }
    if(mDurationMicros == 0){
        return 0.0f;
    }
    if(clamp && isExpired()){
        return 1.0f;
    }  
    return static_cast<float>(getElapsedMicros()) / static_cast<float>(mDurationMicros);
}

}
