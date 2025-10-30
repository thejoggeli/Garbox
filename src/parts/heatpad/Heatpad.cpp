#include "Heatpad.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "global/PinConfig.h"
#include "util/MathUtils.h"

namespace Garbox {

Heatpad::Heatpad() : 
    // init members
    mPwm(PwmPeriodMillis){
    // nothing to do
}

void Heatpad::init(){
    mGpioHeatpadEnable.setup(PinConfig::HeatpadEnable, Gpio::Mode::Output);
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

void Heatpad::setDutyCycle(float duty){
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
    if(state == SoftwarePwm::State::High){
        setHeatEnabled(true);
    } 
    else {
        setHeatEnabled(false);
    }
}

void Heatpad::setHeatEnabled(bool enabled){
    if (mHeatEnabled != enabled) {
        mGpioHeatpadEnable.setValue(enabled);
        mHeatEnabled = enabled;
    }
}

bool Heatpad::isHeatEnabled(){
    return mHeatEnabled;
}

} // namespace