#include "Button.h"
#include "core/time/Time.h"
#include "assert/Assert.h"

namespace Garbox {

Button::Button(){
    // constructor body
}

Button::~Button(){
    TriggerExit("Button", "heap using classes must not be deconstructed");
}

void Button::init(){
    AssertExit(!mInitialized, "Button", "already initialized");

    // init fsm
    mFsm.init(State::Released);
    mFsm.setStateChangedCallback([this](State oldState, State newState){
        handleFsmStateChanged(oldState, newState);
    });

    // set initial transition delays and state hold times
    setPressDebounceMicros(mPressDebounceMicros);
    setReleaseDebounceMicros(mPressDebounceMicros);
    setLongPressMicros(mLongPressMicros);

    mInitialized = true;
}

void Button::tick(bool isPressedRaw){
    if(!mInitialized){
        TriggerDebug("Button", "tick() called before init()");
        return;
    }

    mFsm.tick();

    switch(mFsm.getState()){
    case State::Released:
        handleReleasedState(isPressedRaw);
        break;
    case State::Pressed:
        handlePressedState(isPressedRaw);
        break;
    case State::PressedLong:
        handlePressedLongState(isPressedRaw);
        break;
    default:
        TriggerDebug("Button", "invalid FSM state");
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

void Button::handleMissedPulse(bool isPressedRaw, uint32_t pulseDuration){
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

void Button::handleReleasedState(bool isPressedRaw){
    if(isPressedRaw){
        mFsm.transition(State::Pressed);
    }
    else {
        mFsm.cancelPendingTransition();
    }
}

void Button::handlePressedState(bool isPressedRaw){
    if(!isPressedRaw){
        mFsm.transition(State::Released);
    }
    else {
        mFsm.transition(State::PressedLong);
    }
}

void Button::handlePressedLongState(bool isPressedRaw){
    if(!isPressedRaw){
        mFsm.transition(State::Released);
    }
    else {
        mFsm.cancelPendingTransition();
    }
}
    
void Button::handleFsmStateChanged(State oldState, State newState){

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

void Button::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void Button::setHoldCallback(HoldCallback callback){
    mHoldCallback = callback;
}

void Button::setUserData(void* userData){
    mUserData = userData;
}

void Button::setPressDebounceMicros(uint32_t debounceMicros){
    mPressDebounceMicros = debounceMicros;
    mFsm.setStateHoldTimeMicros(State::Pressed, debounceMicros);
}

void Button::setReleaseDebounceMicros(uint32_t debounceMicros){
    mReleaseDebounceMicros = debounceMicros;
    mFsm.setStateHoldTimeMicros(State::Released, debounceMicros);
}

void Button::setLongPressMicros(uint32_t delayMicros){
    mLongPressMicros = delayMicros;
    mFsm.setTransitionDelayMicros(State::Pressed, State::PressedLong, delayMicros);
}

void Button::setInitialHoldDelayMicros(uint32_t delayMicros){
    mInitialHoldDelayMicros = delayMicros;
}

void Button::setRepeatHoldDelayMicros(uint32_t delayMicros){
    mRepeatHoldDelayMicros = delayMicros;
}

bool Button::isPressed() const {
    State state = mFsm.getState();
    return (state == State::Pressed) || (state == State::PressedLong);
}

bool Button::isLongPressed() const {
    return (mFsm.getState() == State::PressedLong);
}

bool Button::isReleased() const {
    return (mFsm.getState() == State::Released);
}

const char* Button::StateToString(State state){
    switch(state){
    case State::Released: return "Released";
    case State::Pressed: return "Pressed";
    case State::PressedLong: return "PressedLong";
    default: return "Unknown";
    }
}

} // namespace Garbox
