#include "FsmButton.h"
#include "core/time/Time.h"
#include "assert/Assert.h"

namespace Garbox {

FsmButton::FsmButton(){
    // set initial transition delays and state hold times
    setPressedToReleasedDelayMicros(InitialPressedToReleasedDelayMicros);
    setReleasedToPressedDelayMicros(InitialReleasedToPressedDelayMicros);
    setPressedHoldTimeMicros(InitialPressedHoldTimeMicros);
    setReleasedHoldTimeMicros(InitialReleasedHoldTimeMicros);
    setLongPressMicros(InitialLongPressMicros);
}

FsmButton::~FsmButton(){
    TriggerExit("FsmButton", "heap using classes must not be deconstructed");
}

void FsmButton::init(){
    AssertExit(!mInitialized, "FsmButton", "already initialized");

    // init fsm
    mFsm.init(ButtonState::Released);
    mFsm.setStateChangedCallback([this](ButtonState oldState, ButtonState newState){
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
    case ButtonState::Released:
        handleReleasedState();
        break;
    case ButtonState::Pressed:
        handlePressedState();
        break;
    case ButtonState::PressedLong:
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
    const ButtonState currentState = mFsm.getState();
    // Released => Pressed
    if(currentState == ButtonState::Released){
        if(pulseDuration >= mFsm.getTransitionDelayMicros(ButtonState::Released, ButtonState::Pressed)){
            mFsm.forceTransition(ButtonState::Pressed);    
        }
    }
    // Pressed => Released
    else if(currentState == ButtonState::Pressed){
        if(pulseDuration >= mFsm.getTransitionDelayMicros(ButtonState::Pressed, ButtonState::Released)){
            mFsm.forceTransition(ButtonState::Released);
        }
    }
    // PressedLong => Released
    else if(currentState == ButtonState::PressedLong){
        if(pulseDuration >= mFsm.getTransitionDelayMicros(ButtonState::PressedLong, ButtonState::Released)){
            mFsm.forceTransition(ButtonState::Released);
        }
    }
}

void FsmButton::handleReleasedState(){
    if(mIsPressedPhysical){
        mFsm.transition(ButtonState::Pressed);
    }
    else {
        mFsm.cancelPendingTransition();
    }
}

void FsmButton::handlePressedState(){
    if(!mIsPressedPhysical){
        mFsm.transition(ButtonState::Released);
    }
    else {
        mFsm.transition(ButtonState::PressedLong);
    }
}

void FsmButton::handlePressedLongState(){
    if(!mIsPressedPhysical){
        mFsm.transition(ButtonState::Released);
    }
    else {
        mFsm.cancelPendingTransition();
    }
}
    
void FsmButton::handleFsmStateChanged(ButtonState oldState, ButtonState newState){

    // setup hold timer on press
    if((mRepeatHoldDelayMicros > 0) && mHoldCallback){
        const bool wasReleased = (oldState == ButtonState::Released);
        const bool becomesPressed = (newState == ButtonState::Pressed) || (newState == ButtonState::PressedLong);
        if(wasReleased && becomesPressed){
            mHoldCounter = 0;
            mHoldTimer.start(mInitialHoldDelayMicros);
        }
    }

    // reset hold timer on release
    if(mHoldTimer.isRunning() || (mHoldCounter > 0)){
        const bool wasPressed = (oldState == ButtonState::Pressed) || (oldState == ButtonState::PressedLong);
        const bool becomesReleased = (newState == ButtonState::Released);
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
    mFsm.setTransitionDelayMicros(ButtonState::Pressed, ButtonState::Released, micros);
    mFsm.setTransitionDelayMicros(ButtonState::PressedLong, ButtonState::Released, micros);
}

void FsmButton::setReleasedToPressedDelayMicros(uint32_t micros){
    mFsm.setTransitionDelayMicros(ButtonState::Released, ButtonState::Pressed, micros);
}

void FsmButton::setPressedHoldTimeMicros(uint32_t micros){
    mFsm.setStateHoldTimeMicros(ButtonState::Pressed, micros);
    mFsm.setStateHoldTimeMicros(ButtonState::PressedLong, micros);
}

void FsmButton::setReleasedHoldTimeMicros(uint32_t micros){
    mFsm.setStateHoldTimeMicros(ButtonState::Released, micros);
}

void FsmButton::setLongPressMicros(uint32_t delayMicros){
    mLongPressMicros = delayMicros;
    mFsm.setTransitionDelayMicros(ButtonState::Pressed, ButtonState::PressedLong, delayMicros);
}

void FsmButton::setInitialHoldDelayMicros(uint32_t delayMicros){
    mInitialHoldDelayMicros = delayMicros;
}

void FsmButton::setRepeatHoldDelayMicros(uint32_t delayMicros){
    mRepeatHoldDelayMicros = delayMicros;
}

bool FsmButton::isPressed() const {
    ButtonState state = mFsm.getState();
    return (state == ButtonState::Pressed) || (state == ButtonState::PressedLong);
}

bool FsmButton::isLongPressed() const {
    return (mFsm.getState() == ButtonState::PressedLong);
}

bool FsmButton::isReleased() const {
    return (mFsm.getState() == ButtonState::Released);
}

} // namespace Garbox
