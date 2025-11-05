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

    mFsm.init(State::Released);
    mFsm.setStateChangedCallback([this](State oldState, State newState){
            handleFsmStateChanged(oldState, newState);
    });

    // set transition delays and state hold times
    mFsm.setTransitionDelayMicros(State::Released, State::Pressed, mPressDebounceMicros);
    mFsm.setTransitionDelayMicros(State::Pressed, State::Released, mReleaseDebounceMicros);
    mFsm.setTransitionDelayMicros(State::Pressed, State::PressedLong, mLongPressMicros);
    mFsm.setTransitionDelayMicros(State::PressedLong, State::Released, mReleaseDebounceMicros);

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
        if(isPressedRaw){
            mFsm.transition(State::Pressed);
        }
        break;

    case State::Pressed:
        if(!isPressedRaw){
            mFsm.transition(State::Released);
        }
        else {
            mFsm.transition(State::PressedLong);
        }
        break;

    case State::PressedLong:
        if(!isPressedRaw){
            mFsm.transition(State::Released);
        }
        break;

    default:
        TriggerDebug("Button", "invalid FSM state");
        break;
    }

    // handle hold logic
    const bool holdEnabled = (mRepeatHoldDelayMicros > 0);
    if(!holdEnabled){
        return;
    }

    const bool isPressedOrLong = (mFsm.getState() == State::Pressed) ||
                                 (mFsm.getState() == State::PressedLong);

    if(isPressedOrLong && mHoldTimer.isExpired()){
        if(mHoldCallback){
            uint32_t elapsedMicros = Time::GetMicros() - mHoldStartTimeMicros;
            mHoldCallback(mHoldCounter, elapsedMicros, mUserData);
        }
        mHoldCounter++;
        mHoldTimer.restart(mRepeatHoldDelayMicros);
    }
}

void Button::handleFsmStateChanged(State oldState, State newState){

    const bool wasReleased = (oldState == State::Released);
    const bool becomesPressed = (newState == State::Pressed) || (newState == State::PressedLong);
    const bool wasPressed = (oldState == State::Pressed) || (oldState == State::PressedLong);
    const bool becomesReleased = (newState == State::Released);

    // setup hold timer on press
    if(wasReleased && becomesPressed){
        if(mRepeatHoldDelayMicros > 0){
            mHoldCounter = 0;
            mHoldStartTimeMicros = Time::GetMicros();
            mHoldTimer.start(mInitialHoldDelayMicros);

            // first hold fires immediately
            if(mHoldCallback){
                mHoldCallback(mHoldCounter, 0, mUserData);
            }
            mHoldCounter++;
        }
    }

    // reset hold timer on release
    if(wasPressed && becomesReleased){
        mHoldTimer.reset();
    }

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

void Button::setPressDebounceMicros(uint32_t delayMicros){
    mPressDebounceMicros = delayMicros;
    mFsm.setTransitionDelayMicros(State::Released, State::Pressed, delayMicros);
}

void Button::setReleaseDebounceMicros(uint32_t delayMicros){
    mReleaseDebounceMicros = delayMicros;
    mFsm.setTransitionDelayMicros(State::Pressed, State::Released, delayMicros);
    mFsm.setTransitionDelayMicros(State::PressedLong, State::Released, delayMicros);
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
