#include "Heatpad.h"

#include <cmath>
#include "core/assert/Assert.h"
#include "core/hardware/adc/Adc.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

Heatpad::Heatpad(const Config& config): 
    // init members
    mGpioHeatpadEnable(config.enableGpio),
    mVoltageSenseAdc(config.voltageSenseAdc),
    mCurrentSenseAdc(config.currentSenseAdc),
    mVoltageSenseConditioner(config.adcFilterTicks),
    mCurrentSenseConditioner(config.adcFilterTicks),
    mPwm(config.initialPwmDuty, config.initialPwmPeriodMicros){
    // nothing to do
}

void Heatpad::init(){

    // init voltage sense filter
    mVoltageSenseConditioner.setFixedPointScaling(1000.0f); // theoretical 1mV resolution
    mVoltageSenseConditioner.setSnapping(0.1f, 0.8f); // 100mV snapping

    // U_Adc = U_In / AttenuationFactor
    // U_In = U_Adc * AttenuationFactor 
    const float AttenuationFactor = 11.0f;
    const float VoltageAtOneAdcVolt = AttenuationFactor;
    mVoltageSenseConditioner.setCalibrationPoints({0.0f, 0.0f}, {1.0f, VoltageAtOneAdcVolt});

    // init current sense filter
    mCurrentSenseConditioner.setFixedPointScaling(1000.0f); // theoretical 1mA resolution
    mCurrentSenseConditioner.setSnapping(0.1f, 0.8f); // 100mA snapping

    // U_Adc = U_Shunt * AmpFactor => U_Shunt = U_Adc / AmpFactor
    // I_Shunt = U_Shunt / R_Shunt
    // I_Shunt = (U_Adc / AmpFactor) * R_Shunt
    const float R_Shunt = 0.002f;
    const float AmpFactor = 200.0f;
    const float CurrentAtOneAdcVolt = (1.0f / AmpFactor) / R_Shunt; 
    mCurrentSenseConditioner.setCalibrationPoints({0.0f, 0.0f}, {1.0f, CurrentAtOneAdcVolt});
    
    // software pwm attach state changed handler
    mPwm.setStateChangedHandler([this](SoftwarePwm::State state){
        handlePwmStateChanged(state);
    });

    // software pwm setup
    mPwm.setMode(SoftwarePwm::Mode::StartHigh);
}

void Heatpad::tick(){
    mPwm.tick();

    // sample adc voltages
    mVoltageSenseAdc.sample();
    mCurrentSenseAdc.sample();

    // condition adc voltages (filter + transform to actual units)
    mVoltageSenseConditioner.process(mVoltageSenseAdc.getVolts());
    mCurrentSenseConditioner.process(mCurrentSenseAdc.getVolts());
}

void Heatpad::setEnabled(bool enabled){
    if(enabled == mEnabled){
        return;
    }
    mEnabled = enabled;
    if(enabled){
        mPwm.start();
    }
    else {
        mPwm.stop();
        setHeatEnabled(false);
    }
    updateState();
}

void Heatpad::setDutyCycle(float duty){
    bool const finishCurrent = true;
    mPwm.setDutyCycle(duty, finishCurrent);
}

void Heatpad::setStateChangedHandler(StateChangedHandler handler){
    mStateChangedHandler = handler;
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
    updateState();
}

void Heatpad::updateState(){
    HeatpadState oldState = mState;
    HeatpadState newState;
    if(!mEnabled){
        newState = HeatpadState::Disabled;
    }
    else if(isHeating()){
        newState = HeatpadState::Heating;
    }
    else {
        newState = HeatpadState::Idle;
    }
    if(oldState != newState){
        mState = newState;
        if(mStateChangedHandler){
            mStateChangedHandler(oldState, newState);
        }
    }
}

void Heatpad::setHeatEnabled(bool enabled){
    if (mHeatEnabled != enabled){
        mGpioHeatpadEnable.writeLevel(enabled);
        mHeatEnabled = enabled;
    }
}

bool Heatpad::isHeating(){
    return mHeatEnabled;
}

bool Heatpad::isEnabled() const {
    return mPwm.isRunning();
}

HeatpadState Heatpad::getState() const {
    return mState;
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

float Heatpad::getMeasuredVoltage() const {
    return mVoltageSenseConditioner.getFilteredValue();
}

float Heatpad::getMeasuredCurrent() const {
    return mCurrentSenseConditioner.getFilteredValue();
}

} // namespace