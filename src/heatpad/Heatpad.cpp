#include "Heatpad.h"

#include <Arduino.h>

#include "assert/Assert.h"
#include "config/GpioConfig.h"
#include "core/Time.h"
#include "utils/MathUtils.h"

namespace Garbox {

Heatpad::Heatpad() : 
    // init members
    mState(State::Off),
    mGpioHeatpadEnable(GpioConfig::HEATPAD_ENABLE){
    // nothing to do
}

void Heatpad::init(){
    mGpioHeatpadEnable.setMode(Gpio::Mode::Output);
    setDutyCycle(0.0f);
}

void Heatpad::start(){
    // nothing to do
}

void Heatpad::tick(){
    switch(mState){
        case State::Off:
            break;
        case State::DutyHigh:
            if(mPwmTimer.getElapsedMillis() >= mHighDuration){
                // switch to low phase
            }
            break;
        case State::DutyLow:
            if(mPwmTimer.isExpired()){
                // start next cycle
            }
            break;
    }
}

void Heatpad::setDutyCycle(float duty){
    mDutyCycle = MathUtils::clamp(duty, 0.0f, 1.0f);
    if(mDutyCycle == 0.0f){
        setOutput(false);
    }
}

void Heatpad::turnOff(){
    setDutyCycle(0.0f);
    mState = State::Off;
}

void Heatpad::setOutput(bool on){
    mGpioHeatpadEnable.digitalWrite(false);
}

} // namespace