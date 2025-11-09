#include "Heatpad.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "global/PinConfig.h"
#include "global/gpio/GpioInstances.h"
#include "util/math/MathUtils.h"

namespace Garbox {

static constexpr uint32_t InitialPwmDuty = 0.5f;
static constexpr uint32_t InitialPwmPeriodMicros = 5'000'000; // 5 seconds

Heatpad::Heatpad() : 
    // init members
    mGpioHeatpadEnable(GpioInstances::GetHeatEnable()),
    mPwm(InitialPwmDuty, InitialPwmPeriodMicros){
    // nothing to do
}

void Heatpad::init(){
    
    // software pwm attach state changed handler
    mPwm.setStateChangedHandler([this](SoftwarePwm::State state) {
        handlePwmStateChanged(state);
    });

    // software pwm setup
    mPwm.setMode(SoftwarePwm::Mode::StartHigh);
}

void Heatpad::start(){
    // start software pwm
    mPwm.start();
}

void Heatpad::tick(){
    mPwm.tick();
}

void Heatpad::reset(){
    mPwm.stop();
    setHeatEnabled(false);
}

void Heatpad::setDutyCycle(float duty){
    bool const finishCurrent = true;
    mPwm.setDutyCycle(duty, finishCurrent);
}

void Heatpad::setPeriodDurationMicros(float durationMicros){
    bool const finishCurrent = true;
    mPwm.setPeriodDurationMicros(durationMicros, finishCurrent);
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
        mGpioHeatpadEnable.writeLevel(enabled);
        mHeatEnabled = enabled;
    }
}

bool Heatpad::isHeatEnabled(){
    return mHeatEnabled;
}

float Heatpad::getCurrentDutyCycle() const {
    return mPwm.getCurrentDutyCycle();
}

float Heatpad::getNextDutyCycle() const {
    return mPwm.getNextDutyCycle();
}

uint32_t Heatpad::getCurrentPeriodDurationMicros() const {
    return mPwm.getNextPeriodDurationMicros();
}

uint32_t Heatpad::getNextPeriodDurationMicros() const {
    return mPwm.getNextPeriodDurationMicros();
}

} // namespace