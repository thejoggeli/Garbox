#include "FsmButton.h"
#include "core/time/Time.h"
#include "assert/Assert.h"

namespace Garbox {

FsmButton::FsmButton(){
    // set initial transition delays and state hold times
    setPressedToReleasedDelayMicros(InitialPressedToReleasedDelayMicros);
    setReleasedToPressedDelayMicros(InitialReleasedToPressedDelayMicros);
    setLongPressMicros(InitialLongPressMicros);
}

FsmButton::~FsmButton(){
    TriggerExit("FsmButton", "heap using classes must not be deconstructed");
}

void FsmButton::init(){
    AssertExit(!mInitialized, "FsmButton", "already initialized");

    // init fsm
    mFsm.init(State::Released);
    mFsm.setStateChangedCallback([this](State oldState, State newState){
        handleFsmStateChanged(oldState, newState);
    });

    mInitialized = true;
}

void FsmButton::tick(){
    if(!mInitialized){
        TriggerDebug("FsmButton", "tick() called before init()");
        return;
    }

    mFsm.tick();

    switch(mFsm.getState()){
    case State::Released:
        handleReleasedState();
        break;
    case State::Pressed:
        handlePressedState();
        break;
    case State::PressedLong:
        handlePressedLongState();
        break;
    default:
        TriggerDebug("FsmButton", "invalid FSM state");
        break;
    }

    // call handle hold callback
    if(mHoldCallback && mHoldTimer.isExpired()){
        if(mHoldCounter == 0){
            mHoldStartTimeMicros = Time::GetMicros();
        }
        uint32_t elapsedMicros = Time::GetMicros() - mHoldStartTimeMicros;
        mHoldCallback(mHoldCounter, elapsedMicros, mUserData);
        mHoldCounter++;
        mHoldTimer.restart(mRepeatHoldDelayMicros);
    }
}

void FsmButton::setPhysicalButtonState(bool pressed){
    mIsPressedPhysical = pressed;
}

void FsmButton::handleMissedPulse(uint32_t pulseDuration){
    const State currentState = mFsm.getState();
    // Released => Pressed
    if(currentState == State::Released){
        if(pulseDuration >= mFsm.getTransitionDelayMicros(State::Released, State::Pressed)){
            mFsm.forceTransition(State::Pressed);    
        }
    }
    // Pressed => Released
    else if(currentState == State::Pressed){
        if(pulseDuration >= mFsm.getTransitionDelayMicros(State::Pressed, State::Released)){
            mFsm.forceTransition(State::Released);
        }
    }
    // PressedLong => Released
    else if(currentState == State::PressedLong){
        if(pulseDuration >= mFsm.getTransitionDelayMicros(State::PressedLong, State::Released)){
            mFsm.forceTransition(State::Released);
        }
    }
}

void FsmButton::handleReleasedState(){
    if(mIsPressedPhysical){
        mFsm.transition(State::Pressed);
    }
    else {
        mFsm.cancelPendingTransition();
    }
}

void FsmButton::handlePressedState(){
    if(!mIsPressedPhysical){
        mFsm.transition(State::Released);
    }
    else {
        mFsm.transition(State::PressedLong);
    }
}

void FsmButton::handlePressedLongState(){
    if(!mIsPressedPhysical){
        mFsm.transition(State::Released);
    }
    else {
        mFsm.cancelPendingTransition();
    }
}
    
void FsmButton::handleFsmStateChanged(State oldState, State newState){

    // setup hold timer on press
    if((mRepeatHoldDelayMicros > 0) && mHoldCallback){
        const bool wasReleased = (oldState == State::Released);
        const bool becomesPressed = (newState == State::Pressed) || (newState == State::PressedLong);
        if(wasReleased && becomesPressed){
            mHoldCounter = 0;
            mHoldTimer.start(mInitialHoldDelayMicros);
        }
    }

    // reset hold timer on release
    if(mHoldTimer.isRunning() || (mHoldCounter > 0)){
        const bool wasPressed = (oldState == State::Pressed) || (oldState == State::PressedLong);
        const bool becomesReleased = (newState == State::Released);
        if(wasPressed && becomesReleased){
            mHoldCounter = 0;
            mHoldTimer.reset();
        }
    }

    // call state changed callback
    if(mStateChangedCallback){
        mStateChangedCallback(oldState, newState, mUserData);
    }
}

void FsmButton::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void FsmButton::setHoldCallback(HoldCallback callback){
    mHoldCallback = callback;
}

void FsmButton::setUserData(void* userData){
    mUserData = userData;
}

void FsmButton::setPressedToReleasedDelayMicros(uint32_t micros){
    mFsm.setTransitionDelayMicros(State::Pressed, State::Released, micros);
    mFsm.setTransitionDelayMicros(State::PressedLong, State::Released, micros);
}

void FsmButton::setReleasedToPressedDelayMicros(uint32_t micros){
    mFsm.setTransitionDelayMicros(State::Released, State::Pressed, micros);
}

void FsmButton::setPressedHoldTimeMicros(uint32_t micros){
    mFsm.setStateHoldTimeMicros(State::Pressed, micros);
    mFsm.setStateHoldTimeMicros(State::PressedLong, micros);
}

void FsmButton::setReleasedHoldTimeMicros(uint32_t micros){
    mFsm.setStateHoldTimeMicros(State::Released, micros);
}

void FsmButton::setLongPressMicros(uint32_t delayMicros){
    mLongPressMicros = delayMicros;
    mFsm.setTransitionDelayMicros(State::Pressed, State::PressedLong, delayMicros);
}

void FsmButton::setInitialHoldDelayMicros(uint32_t delayMicros){
    mInitialHoldDelayMicros = delayMicros;
}

void FsmButton::setRepeatHoldDelayMicros(uint32_t delayMicros){
    mRepeatHoldDelayMicros = delayMicros;
}

bool FsmButton::isPressed() const {
    State state = mFsm.getState();
    return (state == State::Pressed) || (state == State::PressedLong);
}

bool FsmButton::isLongPressed() const {
    return (mFsm.getState() == State::PressedLong);
}

bool FsmButton::isReleased() const {
    return (mFsm.getState() == State::Released);
}

} // namespace Garbox
