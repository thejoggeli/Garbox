#include "Heatpad.h"

#include <Arduino.h>

#include "assert/Assert.h"
#include "config/GpioConfig.h"
#include "core/Time.h"
#include "utils/MathUtils.h"

namespace Garbox {

Heatpad::Heatpad() : 
    // init members
    mGpioHeatpadEnable(GpioConfig::HEATPAD_ENABLE),
    mPwmTimer(){
    // nothing to do
}

void Heatpad::init(){
    mGpioHeatpadEnable.setMode(Gpio::Mode::Output);
    setHeatEnabled(false);
}

void Heatpad::start(){
    // nothing to do
}

void Heatpad::tick(){
    updateStateMachine();
}

void Heatpad::updateStateMachine(){

    switch(mState){
        case State::Off:
            if(mHeatEnabled){
                setHeatEnabled(false);
            }
            break;
        case State::DutyHigh:
            if(mPwmTimer.getElapsedMillis() >= mPwmHighDuration){
                enterPwmLowState();
            } else if(!mHeatEnabled){
                setHeatEnabled(true);
            }
            break;
        case State::DutyLow:
            if(mPwmTimer.isExpired()){
                enterNextPwmCycle();
            } else if(mHeatEnabled){
                setHeatEnabled(false);
            }
            break;
        default:
            AssertDebug(false, "Heatpad:updateStateMachine() unhandled state");
            break;
    }
}

void Heatpad::enterOffState(){
    mState = State::Off;
    mPwmTimer.reset();
    updatePwmDutyCycle(0.0f);
    setHeatEnabled(false);
    updateStateMachine();
}

void Heatpad::enterPwmHighState(){
    mState = State::DutyHigh;
    updateStateMachine();
}

void Heatpad::enterPwmLowState(){
    mState = State::DutyLow;
    updateStateMachine();
}

void Heatpad::enterNextPwmCycle(){
    // apply new duty cycle 
    if(mNextDutyCycle != mCurrentDutyCycle){
        updatePwmDutyCycle(mNextDutyCycle);
    }

    // enter next state
    if(mNextDutyCycle == 0.0f){
        // entter off state
        mPwmTimer.reset();
        enterOffState();
    }
    else {
        // enter next pwm cycle
        mPwmTimer.restart();
        enterPwmHighState();
    }
}

void Heatpad::setNextDutyCycle(float duty){

    if(duty < 0.0f || duty > 1.0f){
        AssertDebug(false, "Heatpad::setNextDutyCycle() invalid duty value");
        return; 
    }

    if(mState == State::Off && duty != 0.0f){
        // turn on
        updatePwmDutyCycle(duty);
        mPwmTimer.start(PwmTimerIntervalMillis);
        enterPwmHighState();
    }
    else {
        // change duty cycle after finishing current cycle
        mNextDutyCycle = duty;
    }

}

float Heatpad::getCurrentDutyCycle(){
    return mCurrentDutyCycle;
}

float Heatpad::getNextDutyCycle(){
    return mNextDutyCycle;
}

void Heatpad::reset(){
    enterOffState();
}

void Heatpad::setHeatEnabled(bool enabled){
    mGpioHeatpadEnable.digitalWrite(enabled);
    mHeatEnabled = enabled;
}

bool Heatpad::isHeatEnabled(){
    return mHeatEnabled;
}

void Heatpad::updatePwmDutyCycle(float duty){
    mCurrentDutyCycle = duty;
    mNextDutyCycle = duty;
    mPwmHighDuration = static_cast<uint32_t>(static_cast<float>(PwmTimerIntervalMillis) * duty);
    mPwmLowDuration = PwmTimerIntervalMillis - mPwmHighDuration;
}

} // namespace