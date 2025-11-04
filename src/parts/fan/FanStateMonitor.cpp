#include "FanStateMonitor.h"

#include "assert/Assert.h"
#include "core/time/Time.h"

namespace Garbox {

FanStateMonitor::FanStateMonitor(
    uint32_t stallThresholdMicros,
    uint32_t stalledCallbackPeriodMicros,
    uint32_t minRpmThreshold,
    uint32_t reenterStallCooldownMicros):
    // initialize members
    mStallThresholdMicros(stallThresholdMicros),
    mStalledCallbackPeriodMicros(stalledCallbackPeriodMicros),
    mReenterStallCooldownMicros(reenterStallCooldownMicros),
    mMinRpmThreshold(minRpmThreshold){
    // nothing to do
}

void FanStateMonitor::init(){
    AssertExit(!mInitialized, "FanStateMonitor", "already initialized");

    // initialize timers and state
    mStallTimer.reset();
    mPeriodicStallTimer.reset();
    mReenterCooldownTimer.reset();
    mState = State::Off;

    mInitialized = true;
}

void FanStateMonitor::setCallback(Callback callback){
    mCallback = callback;
}

void FanStateMonitor::setStalledCallbackPeriod(uint32_t periodMicros){
    mStalledCallbackPeriodMicros = periodMicros;
}

void FanStateMonitor::setStallThreshold(uint32_t stallThresholdMicros){
    mStallThresholdMicros = stallThresholdMicros;
}

void FanStateMonitor::setMinRpmThreshold(uint32_t rpmThreshold){
    mMinRpmThreshold = rpmThreshold;
}

void FanStateMonitor::setReenterStallCooldown(uint32_t cooldownMicros){
    mReenterStallCooldownMicros = cooldownMicros;
}

void FanStateMonitor::reset(){
    if(!mInitialized){
        FailDebug("FanStateMonitor", "not initialized");
    }

    mStallTimer.reset();
    mPeriodicStallTimer.reset();
    mReenterCooldownTimer.reset();
    mState = State::Off;

    if(mCallback){
        mCallback(mState);
    }
}

FanStateMonitor::State FanStateMonitor::getState() const {
    return mState;
}

void FanStateMonitor::tick(uint32_t rpmValue, bool shouldRun){
    if(!mInitialized){
        FailDebug("FanStateMonitor", "tick() called before init()");
    }

    // reset cooldown timer if expired to avoid long-term overflow
    if(mReenterCooldownTimer.isExpired()){
        mReenterCooldownTimer.reset();
    }

    switch(mState){
    case State::Off:
        handleOffState(rpmValue, shouldRun);
        break;

    case State::On:
        handleOnState(rpmValue, shouldRun);
        break;

    case State::Stalled:
        handleStalledState(rpmValue, shouldRun);
        break;

    default:
        FailDebug("FanStateMonitor", "invalid state");
        return;
    }
}

void FanStateMonitor::handleOffState(uint32_t rpmValue, bool shouldRun){
    // when off, switch to on immediately when fan is commanded to run
    if(shouldRun){
        enterState(State::On);
    }
}

void FanStateMonitor::handleOnState(uint32_t rpmValue, bool shouldRun){
    // leave on state if fan is turned off
    if(!shouldRun){
        enterState(State::Off);
        return;
    }

    // fan commanded on but not spinning above threshold
    if(rpmValue < mMinRpmThreshold){
        // block re-entering stalled state if cooldown is active
        if(mReenterStallCooldownMicros > 0 && mReenterCooldownTimer.isRunningAndNotExpired()){
            return;
        }

        // immediate stall if threshold is zero
        if(mStallThresholdMicros == 0){
            enterState(State::Stalled);
            return;
        }

        if(mStallTimer.isReset()){
            mStallTimer.start(mStallThresholdMicros);
        }
        else if(mStallTimer.isExpired()){
            enterState(State::Stalled);
        }
    }
    else if(mStallTimer.isRunning()){
        // running again, reset stall timer if active
        mStallTimer.reset();
    }
}

void FanStateMonitor::handleStalledState(uint32_t rpmValue, bool shouldRun){
    // leave stalled if fan is turned off
    if(!shouldRun){
        enterState(State::Off);
        return;
    }

    // leave stalled if fan exceeds rpm threshold
    if(rpmValue >= mMinRpmThreshold){
        enterState(State::On);
        return;
    }

    // periodic stalled callback (e.g. 0.5 s for 2 Hz beep)
    if(mStalledCallbackPeriodMicros > 0 && mPeriodicStallTimer.isExpired()){
        if(mCallback){
            mCallback(State::Stalled);
        }
        mPeriodicStallTimer.restart();
    }
}

void FanStateMonitor::enterState(State newState){
    // ignore redundant transition and log error
    if(mState == newState){
        FailDebug("FanStateMonitor", "attempted to enter same state");
        return;
    }

    // leave current state
    switch(mState){
    case State::Off:
        onLeaveOffState();
        break;
    case State::On:
        onLeaveOnState();
        break;
    case State::Stalled:
        onLeaveStalledState();
        break;
    default:
        break;
    }

    // update state
    mState = newState;

    // enter new state
    switch(newState){
    case State::Off:
        onEnterOffState();
        break;
    case State::On:
        onEnterOnState();
        break;
    case State::Stalled:
        onEnterStalledState();
        break;
    default:
        break;
    }

    // notify listener once on each transition
    if(mCallback){
        mCallback(mState);
    }
}

// ---------------------------------------------------------
// state entry and exit helpers
// ---------------------------------------------------------

void FanStateMonitor::onEnterOffState(){
    // reset timers when entering off state
    mStallTimer.reset();
    mPeriodicStallTimer.reset();
}

void FanStateMonitor::onLeaveOffState(){
    // nothing
}

void FanStateMonitor::onEnterOnState(){
    // reset stall timer when entering on state
    mStallTimer.reset();
}

void FanStateMonitor::onLeaveOnState(){
    // nothing
}

void FanStateMonitor::onEnterStalledState(){
    // reset timers and start periodic timer if configured
    mStallTimer.reset();
    mPeriodicStallTimer.reset();

    if(mStalledCallbackPeriodMicros > 0){
        mPeriodicStallTimer.start(mStalledCallbackPeriodMicros);
    }
}

void FanStateMonitor::onLeaveStalledState(){
    // start re-entry cooldown timer if configured
    if(mReenterStallCooldownMicros > 0){
        mReenterCooldownTimer.start(mReenterStallCooldownMicros);
    }

    // reset stall-related timers
    mStallTimer.reset();
    mPeriodicStallTimer.reset();
}

} // namespace Garbox
