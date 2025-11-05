#include "FiniteStateMachine.h"

namespace Garbox {

FiniteStateMachine::FiniteStateMachine(uint8_t numStates) : mNumStates(numStates) {

    // zero-initialize transition delay matrix
    mTransitionDelayMicros = new uint32_t*[mNumStates];
    for(uint8_t i = 0; i < mNumStates; ++i){
        mTransitionDelayMicros[i] = new uint32_t[mNumStates];
        std::memset(mTransitionDelayMicros[i], 0, mNumStates * sizeof(uint32_t));
    }

    // zero-initialize state hold time vector
    mStateHoldTimeMicros = new uint32_t[mNumStates];
    std::memset(mStateHoldTimeMicros, 0, mNumStates * sizeof(uint32_t));
}

FiniteStateMachine::~FiniteStateMachine(){
    TriggerExit("FiniteStateMachine", "class uses heap, ~ not allowed");
}

void FiniteStateMachine::init(uint8_t initialState){
    AssertExit(!mInitialized, "FiniteStateMachine", "already initialized");
    mCurrentState = initialState;
    mPendingState = initialState;
    mTransitionTimer.reset();
    mStateHoldTimer.start(mStateHoldTimeMicros[mCurrentState]);
    mInitialized = true;
}

void FiniteStateMachine::tick(){
    if(!mInitialized){
        TriggerDebug("FiniteStateMachine", "not initialized");
        return;
    }

    // state hold timer expired, reset to signal state change is now possible
    if(mStateHoldTimer.isExpired()){
        mStateHoldTimer.reset();
    }

    // apply pending transition if all conditions are met
    const bool stateHoldTimerReady = mStateHoldTimer.isReset();
    const bool transitionTimerReady = (mTransitionTimer.isExpired() || mTransitionTimer.isReset());
    if(hasPendingTransition() && stateHoldTimerReady && transitionTimerReady){
        applyTransition(mPendingState);
    } 

}

void FiniteStateMachine::setTransitionDelayMicros(uint8_t from, uint8_t to, uint32_t delayMicros){
    if((from >= mNumStates) || (to >= mNumStates)){
        TriggerDebug("FiniteStateMachine", "invalid transition index");
        return;
    }
    mTransitionDelayMicros[from][to] = delayMicros;
}

void FiniteStateMachine::setStateHoldTimeMicros(uint8_t state, uint32_t holdMicros){
    if(state >= mNumStates){
        TriggerDebug("FiniteStateMachine", "invalid state index");
        return;
    }
    mStateHoldTimeMicros[state] = holdMicros;
}

void FiniteStateMachine::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void FiniteStateMachine::transition(uint8_t newState){
    if(!mInitialized){
        TriggerDebug("FiniteStateMachine", "not initialized");
        return;
    }

    // abort if new state equals current state
    if(newState == mCurrentState){
        return;
    }

    // transition logic
    const uint32_t delayMicros = mTransitionDelayMicros[mCurrentState][newState];
    if(delayMicros == 0){
        // immediate transition
        if(!mStateHoldTimer.isRunningAndNotExpired()){
            applyTransition(newState);
        }
    }
    else if(mPendingState != newState){
        // set pending transition
        mPendingState = newState;
        mTransitionTimer.start(delayMicros);
    }

}

void FiniteStateMachine::forceTransition(uint8_t newState){
    if(!mInitialized){
        TriggerDebug("FiniteStateMachine", "not initialized");
        return;
    }
    if(newState == mCurrentState){
        return;
    }
    applyTransition(newState);
}

void FiniteStateMachine::applyTransition(uint8_t newState){
    uint8_t oldState = mCurrentState;
    mCurrentState = newState;
    mPendingState = newState;

    // reset transition timer
    mTransitionTimer.reset();

    // start state hold timer
    const uint32_t holdTime = mStateHoldTimeMicros[mCurrentState];
    if(holdTime > 0){
        mStateHoldTimer.start(holdTime);
    }
    else {
        mStateHoldTimer.reset();
    }

    // call state changed callback
    if(mStateChangedCallback){
        mStateChangedCallback(oldState, newState);
    }
}

bool FiniteStateMachine::hasPendingTransition(){
    return (mCurrentState != mPendingState);
}

void FiniteStateMachine::cancelPendingTransition(){
    mPendingState = mCurrentState;
    mTransitionTimer.reset();
}

} // namespace Garbox
