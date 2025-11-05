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
        TriggerDebug("FiniteStateMachine", "tick() called before init()");
        return;
    }

    if(mTransitionTimer.isExpired() && (mPendingState != mCurrentState)){
        applyTransition(mPendingState);
        mTransitionTimer.reset();
    }

    if(mStateHoldTimer.isExpired()){
        mStateHoldTimer.reset();
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

    if(newState == mCurrentState){
        return;
    }

    if(!mStateHoldTimer.isExpired()){
        return;
    }

    uint32_t delayMicros = mTransitionDelayMicros[mCurrentState][newState];
    if(delayMicros == 0){
        applyTransition(newState);
    }
    else {
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
    mStateHoldTimer.start(mStateHoldTimeMicros[mCurrentState]);
    if(mStateChangedCallback){
        mStateChangedCallback(oldState, newState);
    }
}

} // namespace Garbox
