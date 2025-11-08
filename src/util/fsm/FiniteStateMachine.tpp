#include "FiniteStateMachine.h"

#include "assert/Assert.h"

namespace Garbox {

template<typename StateType, StateType NumStates>
FiniteStateMachine<StateType, NumStates>::FiniteStateMachine() {
    AssertExit(static_cast<size_t>(NumStates) > 0, "FiniteStateMachine", "invalid number of states");
}

template<typename StateType, StateType NumStates>
FiniteStateMachine<StateType, NumStates>::~FiniteStateMachine(){
    AssertExit(static_cast<size_t>(NumStates) > 0, "FiniteStateMachine", "std::function might use heap");
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::init(StateType initialState){
    AssertExit(!mInitialized, "FiniteStateMachine", "already initialized");
    mCurrentState = initialState;
    mPendingState = initialState;
    mTransitionTimer.reset();
    mStateHoldTimer.start(mStateHoldTimeMicros[static_cast<size_t>(mCurrentState)]);
    mInitialized = true;
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::tick(){
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
    const bool shouldTransition = hasPendingTransition() && stateHoldTimerReady && transitionTimerReady;
    if(shouldTransition){
        applyTransition(mPendingState);
    }
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::setTransitionDelayMicros(StateType from, StateType to, uint32_t delayMicros){
    if((from >= NumStates) || (to >= NumStates)){
        TriggerDebug("FiniteStateMachine", "invalid transition index");
        return;
    }
    mTransitionDelayMicros[static_cast<size_t>(from)][static_cast<size_t>(to)] = delayMicros;
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::setStateHoldTimeMicros(StateType state, uint32_t holdMicros){
    if(state >= NumStates){
        TriggerDebug("FiniteStateMachine", "invalid state index");
        return;
    }
    mStateHoldTimeMicros[static_cast<size_t>(state)] = holdMicros;
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::transition(StateType newState){
    if(!mInitialized){
        TriggerDebug("FiniteStateMachine", "not initialized");
        return;
    }

    // abort if new state equals current state or is already pending
    if((newState == mCurrentState) || (newState == mPendingState)){
        return;
    }

    // transition logic
    const uint32_t delayMicros = mTransitionDelayMicros[static_cast<size_t>(mCurrentState)][static_cast<size_t>(newState)];
    if(delayMicros == 0){
        // no transition delay
        const bool holdTimerActive = mStateHoldTimer.isRunningAndNotExpired();
        if(holdTimerActive){
            // hold timer still active => set pending transition without transition timer
            mPendingState = newState;
            mTransitionTimer.reset();
        }
        else {
            // hold timer not active => immediate transition
            applyTransition(newState);
        }
    }
    else {
        // transiton delay => set pending transition with transition timer
        mPendingState = newState;
        mTransitionTimer.start(delayMicros);
    }

}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::forceTransition(StateType newState){
    if(!mInitialized){
        TriggerDebug("FiniteStateMachine", "not initialized");
        return;
    }
    if(newState == mCurrentState){
        return;
    }
    applyTransition(newState);
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::applyTransition(StateType newState){
    StateType oldState = mCurrentState;
    mCurrentState = newState;
    mPendingState = newState;

    // reset transition timer
    mTransitionTimer.reset();

    // start state hold timer
    const uint32_t holdTime = mStateHoldTimeMicros[static_cast<size_t>(mCurrentState)];
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

template<typename StateType, StateType NumStates>
bool FiniteStateMachine<StateType, NumStates>::hasPendingTransition(){
    return (mCurrentState != mPendingState);
}

template<typename StateType, StateType NumStates>
void FiniteStateMachine<StateType, NumStates>::cancelPendingTransition(){
    mPendingState = mCurrentState;
    mTransitionTimer.reset();
}

template<typename StateType, StateType NumStates>
StateType FiniteStateMachine<StateType, NumStates>::getState() const {
    return mCurrentState;
}

template<typename StateType, StateType NumStates>
uint32_t FiniteStateMachine<StateType, NumStates>::getTransitionDelayMicros(StateType from, StateType to) const {
    if((from >= NumStates) || (to >= NumStates)){
        TriggerDebug("FiniteStateMachine", "invalid transition index");
        return 0;
    }
    return mTransitionDelayMicros[static_cast<size_t>(from)][static_cast<size_t>(to)];
}

template<typename StateType, StateType NumStates>
uint32_t FiniteStateMachine<StateType, NumStates>::getStateHoldTimeMicros(StateType state) const {
    if(state >= NumStates){
        TriggerDebug("FiniteStateMachine", "invalid transition index");
        return 0;
    }
    return mStateHoldTimeMicros[static_cast<size_t>(state)];
}


} // namespace Garbox
