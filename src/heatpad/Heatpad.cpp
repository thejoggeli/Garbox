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
    mPwm(PwmPeriodMillis){
    // nothing to do
}

void Heatpad::init(){
    mGpioHeatpadEnable.setMode(Gpio::Mode::Output);
    setHeatEnabled(false);

    // attach pwm state changed handler
    mPwm.setStateChangedHandler([this](SoftwarePwm::State state) {
        handlePwmStateChanged(state);
    });
}

void Heatpad::start(){
    // nothing to do
}

void Heatpad::tick(){
    mPwm.tick();
}

void Heatpad::reset(){
    mPwm.reset();
    setHeatEnabled(false);
}

void Heatpad::setNextDutyCycle(float duty){
    bool const finishCurrent = true;
    mPwm.setDutyCycle(duty, finishCurrent);
}

float Heatpad::getCurrentDutyCycle(){
    return mPwm.getCurrentDutyCycle();
}

float Heatpad::getNextDutyCycle(){
    return mPwm.getNextDutyCycle();
}

void Heatpad::handlePwmStateChanged(SoftwarePwm::State state){
    switch(state){
        case SoftwarePwm::State::Off:
        case SoftwarePwm::State::Low:
            setHeatEnabled(false);
            break;
        case SoftwarePwm::State::High:
            setHeatEnabled(true);
            break;
        default: 
            setHeatEnabled(false);
            AssertDebug(false, "Heatpad::handlePwmStateChanged() unhandled state");
            break;
    }
}

void Heatpad::setHeatEnabled(bool enabled){
    if (mHeatEnabled != enabled) {
        mGpioHeatpadEnable.digitalWrite(enabled);
        mHeatEnabled = enabled;
    }
}

bool Heatpad::isHeatEnabled(){
    return mHeatEnabled;
}

} // namespace