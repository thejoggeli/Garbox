#include "FanMonitor.h"

#include "assert/Assert.h"
#include "core/time/Time.h"

namespace Garbox {

FanMonitor::FanMonitor(
    uint32_t idleStallThresholdMicros,
    uint32_t spinningStallThresholdMicros,
    uint32_t stalledAlertPeriodMicros,
    uint32_t minRpmThreshold,
    uint32_t reenterStallCooldownMicros):
    // initialize members
    mIdleStallThresholdMicros(idleStallThresholdMicros),
    mSpinningStallThresholdMicros(spinningStallThresholdMicros),
    mStalledAlertPeriodMicros(stalledAlertPeriodMicros),
    mReenterStallCooldownMicros(reenterStallCooldownMicros),
    mMinRpmThreshold(minRpmThreshold){
    // nothing to do
}

void FanMonitor::init(){
    AssertExit(!mInitialized, "FanMonitor", "already initialized");

    mStallTimer.reset();
    mPeriodicAlertTimer.reset();
    mReenterCooldownTimer.reset();
    mState = State::Idle;
    mInitialized = true;
}

void FanMonitor::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void FanMonitor::setStalledAlertCallback(StalledAlertCallback callback){
    mStalledAlertCallback = callback;
}

void FanMonitor::setStalledAlertPeriod(uint32_t periodMicros){
    mStalledAlertPeriodMicros = periodMicros;
}

void FanMonitor::setIdleStallThreshold(uint32_t stallThresholdMicros){
    mIdleStallThresholdMicros = stallThresholdMicros;
}

void FanMonitor::setSpinningStallThreshold(uint32_t stallThresholdMicros){
    mSpinningStallThresholdMicros = stallThresholdMicros;
}

void FanMonitor::setMinRpmThreshold(uint32_t rpmThreshold){
    mMinRpmThreshold = rpmThreshold;
}

void FanMonitor::setReenterStallCooldown(uint32_t cooldownMicros){
    mReenterStallCooldownMicros = cooldownMicros;
}

void FanMonitor::reset(){
    if(!mInitialized){
        TriggerDebug("FanMonitor", "not initialized");
        return;
    }

    mStallTimer.reset();
    mPeriodicAlertTimer.reset();
    mReenterCooldownTimer.reset();
    mState = State::Idle;

    if(mStateChangedCallback){
        mStateChangedCallback(State::Idle);
    }
}

FanMonitor::State FanMonitor::getState() const {
    return mState;
}

void FanMonitor::tick(uint32_t rpmValue, bool shouldSpin){
    if(!mInitialized){
        TriggerDebug("FanMonitor", "tick() called before init()");
        return;
    }

    if(mReenterCooldownTimer.isExpired()){
        mReenterCooldownTimer.reset();
    }

    switch(mState){
    case State::Idle:
        handleIdleState(rpmValue, shouldSpin);
        break;
    case State::Spinning:
        handleSpinningState(rpmValue, shouldSpin);
        break;
    case State::Stalled:
        handleStalledState(rpmValue, shouldSpin);
        break;
    default:
        TriggerDebug("FanMonitor", "invalid state");
        return;
    }
}

void FanMonitor::handleIdleState(uint32_t rpmValue, bool shouldSpin){
    if(rpmValue >= mMinRpmThreshold){
        enterState(State::Spinning);
    }
    else if(shouldSpin){
        if(mIdleStallThresholdMicros == 0){
            enterState(State::Stalled);
        } 
        else if(mStallTimer.isReset()){
            mStallTimer.start(mIdleStallThresholdMicros);
        }
        else if(mStallTimer.isExpired()){
            enterState(State::Stalled);
        }
    } 
    else if(mStallTimer.isRunning()){
        mStallTimer.reset();
    }
}

void FanMonitor::handleSpinningState(uint32_t rpmValue, bool shouldSpin){
    if(rpmValue < mMinRpmThreshold){
        if(!shouldSpin){
            enterState(State::Idle);
        } 
        else if((mReenterStallCooldownMicros) > 0 && mReenterCooldownTimer.isRunningAndNotExpired()){
            // in reenter timeout
        } 
        else if(mSpinningStallThresholdMicros == 0){
            enterState(State::Stalled);
        } 
        else if(mStallTimer.isReset()){
            mStallTimer.start(mSpinningStallThresholdMicros);
        }
        else if(mStallTimer.isExpired()){
            enterState(State::Stalled);
        }
    }
    else if(mStallTimer.isRunning()){
        mStallTimer.reset();
    }
}

void FanMonitor::handleStalledState(uint32_t rpmValue, bool shouldSpin){
    if(!shouldSpin){
        enterState(State::Idle);
        return;
    }

    if(rpmValue >= mMinRpmThreshold){
        enterState(State::Spinning);
        return;
    }

    if(mStalledAlertPeriodMicros > 0 && mPeriodicAlertTimer.isExpired()){
        if(mStalledAlertCallback){
            mStalledAlertCallback(mStallCounter++);
        }
        mPeriodicAlertTimer.restart();
    }
}

void FanMonitor::enterState(State newState){
    if(mState == newState){
        TriggerDebug("FanMonitor", "attempted to enter same state");
        return;
    }

    switch(mState){
    case State::Idle: onLeaveIdleState(); break;
    case State::Spinning: onLeaveSpinningState(); break;
    case State::Stalled: onLeaveStalledState(); break;
    default: break;
    }

    mState = newState;

    switch(newState){
    case State::Idle: onEnterIdleState(); break;
    case State::Spinning: onEnterSpinningState(); break;
    case State::Stalled: onEnterStalledState(); break;
    default: break;
    }

    if(mStateChangedCallback){
        mStateChangedCallback(mState);
    }
}

void FanMonitor::onEnterIdleState(){
    mStallTimer.reset();
    mPeriodicAlertTimer.reset();
}

void FanMonitor::onLeaveIdleState(){
    // nothing to do
}

void FanMonitor::onEnterSpinningState(){
    mStallTimer.reset();
}

void FanMonitor::onLeaveSpinningState(){
    // nothing to do
}

void FanMonitor::onEnterStalledState(){
    mStallTimer.reset();
    mPeriodicAlertTimer.reset();

    if(mStalledAlertPeriodMicros > 0){
        mPeriodicAlertTimer.start(mStalledAlertPeriodMicros);
        if(mStalledAlertCallback){
            mStalledAlertCallback(mStallCounter++);
        }
    }
}

void FanMonitor::onLeaveStalledState(){
    if(mReenterStallCooldownMicros > 0){
        mReenterCooldownTimer.start(mReenterStallCooldownMicros);
    }
    mStallCounter = 0;
    mStallTimer.reset();
    mPeriodicAlertTimer.reset();
}

} // namespace Garbox
