#include "FanMonitor.h"
#include "assert/Assert.h"

namespace Garbox {

FanMonitor::FanMonitor():
    // initialize members
    mFsm(StateToUint(State::Count)){
    // constructor body
}

void FanMonitor::init(){
    AssertExit(!mInitialized, "FanMonitor", "already initialized");

    // 
    mFsm.init(StateToUint(State::Idle));
    mFsm.setStateChangedCallback([this](uint8_t oldState, uint8_t newState){
        handleFsmStateChanged(oldState, newState);
    });

    mInitialized = true;
}

void FanMonitor::tick(uint32_t rpmValue, bool shouldSpin){
    if(!mInitialized){
        TriggerDebug("FanMonitor", "not initialized");
        return;
    }

    mFsm.tick();

    switch(UintToState(mFsm.getState())){
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
        TriggerDebug("FanMonitor", "invalid FSM state");
        return;
    }

    // handle periodic alert
    const bool isStalledState = (mFsm.getState() == StateToUint(State::Stalled));
    if(isStalledState && mStalledAlertCallback && (mStalledAlertPeriodMicros > 0) && mStalledAlertTimer.isExpired()){
        mStalledAlertCallback(mStallCounter);
        mStallCounter++;
        mStalledAlertTimer.restart(mStalledAlertPeriodMicros);
    }
    
}

void FanMonitor::handleIdleState(uint32_t rpmValue, bool shouldSpin){
    const bool isSpinning = (rpmValue >= mMinRpmThreshold);
    if(isSpinning){
        // isSpinning => Spinning (transition)
        mFsm.transition(StateToUint(State::Spinning));
    }
    else if(shouldSpin){
        // !isSpinning && shouldSpin => Staled (transition)
        mFsm.transition(StateToUint(State::Stalled));
    }
    else {
        // !isSpinning && !shouldSpin => Idle (stay)
        mFsm.cancelPendingTransition();
    }
}

void FanMonitor::handleSpinningState(uint32_t rpmValue, bool shouldSpin){
    const bool isSpinning = (rpmValue >= mMinRpmThreshold);
    if(!isSpinning){
        if(shouldSpin){
            // !isSpinning && shouldSPin => Stalled (transition)
            mFsm.transition(StateToUint(State::Stalled));
        } 
        else {
            // !isSpinning && !shouldSPin => Idle (transition)
            mFsm.transition(StateToUint(State::Idle));
        }
    }
    else {
        // isSpinning => Spinning (stay)
        mFsm.cancelPendingTransition();
    }
}

void FanMonitor::handleStalledState(uint32_t rpmValue, bool shouldSpin){
    const bool isSpinning = (rpmValue >= mMinRpmThreshold);
    if(isSpinning){
        // isSpinning => Spinning (transition)
        mFsm.transition(StateToUint(State::Spinning));
    }
    else if(!shouldSpin){
        // !isSpinning && !shouldSpin => Idle (transition)
        mFsm.transition(StateToUint(State::Idle));
    }
    else {
        // should spin but is not spinning => Stalled (stay)
        mFsm.cancelPendingTransition();
    }
}

void FanMonitor::handleFsmStateChanged(uint8_t oldState, uint8_t newState){

    // leave Stalled
    if(UintToState(oldState) == State::Stalled){
        mStallCounter = 0;
        mStalledAlertTimer.reset();
    }

    // enter Stalled
    if(UintToState(newState) == State::Stalled){
        if(mStalledAlertPeriodMicros > 0){
            mStalledAlertTimer.start(mStalledAlertPeriodMicros);
        }
    }

    // call state changed callback
    if(mStateChangedCallback){
        mStateChangedCallback(UintToState(oldState), UintToState(newState));
    }
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

void FanMonitor::setMinRpmThreshold(uint32_t rpmThreshold){
    mMinRpmThreshold = rpmThreshold;
}

void FanMonitor::setTransitionDelay(State from, State to, uint32_t delayMicros){
    mFsm.setTransitionDelayMicros(StateToUint(from), StateToUint(to), delayMicros);
}

const char* FanMonitor::StateToString(State state){
    switch(state){
    case State::Idle: return "Idle";
    case State::Spinning: return "Spinning";
    case State::Stalled: return "Stalled";
    }
    return "Unknown";
}

} // namespace Garbox
