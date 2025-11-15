#include "Heatpad.h"

#include <cmath>
#include "assert/Assert.h"
#include "core/hardware/adc/Adc.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/config/AppConfig.h"
#include "util/math/MathUtils.h"

namespace Garbox {

static constexpr uint32_t AdcConditionerWindowSize = static_cast<uint32_t>(AppConfig::MainTaskFrequencyHz * 1.0f);

static constexpr uint32_t InitialPwmDuty = 0.5f;
static constexpr uint32_t InitialPwmPeriodMicros = 5'000'000; // 5 seconds

Heatpad::Heatpad(Gpio& enableGpio, Adc& voltageSenseAdc, Adc& currentSenseAdc): 
    // init members
    mGpioHeatpadEnable(enableGpio),
    mVoltageSenseAdc(voltageSenseAdc),
    mCurrentSenseAdc(currentSenseAdc),
    mVoltageSenseConditioner(AdcConditionerWindowSize),
    mCurrentSenseConditioner(AdcConditionerWindowSize),
    mPwm(InitialPwmDuty, InitialPwmPeriodMicros){
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

void Heatpad::start(){
    // start software pwm
    mPwm.start();

    // start log timer
    mLogTimer.start(250_ms);
}

void Heatpad::tick(){
    mPwm.tick();

    // sample adc voltages
    mVoltageSenseAdc.sample();
    mCurrentSenseAdc.sample();

    // condition adc voltages (filter + transform to actual units)
    mVoltageSenseConditioner.process(mVoltageSenseAdc.getVolts());
    mCurrentSenseConditioner.process(mCurrentSenseAdc.getVolts());

    // log adc voltages
    if(mLogTimer.isExpired()){
        
        // get most recent adc voltages
        const float voltage = mVoltageSenseConditioner.getFilteredValue();
        const float current = mCurrentSenseConditioner.getFilteredValue();

        static float oldVoltage = 0.0f;
        static float oldCurrent = 0.0f;

        // print usense and isense values if changed
        if((oldVoltage != voltage) || (oldCurrent != current)){
            const float power = std::round(voltage * current / 0.1f) * 0.1f;
            oldVoltage = voltage;
            oldCurrent = current;
            LogDebug("Heatpad", "usense=%4.1f V, isense=%3.1f A, psense=%4.1f W", voltage, current, power);
        }

        // restart timer
        mLogTimer.restart();
    }
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
    if (mHeatEnabled != enabled){
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

float Heatpad::getMeasuredVoltage() const {
    return mVoltageSenseConditioner.getFilteredValue();
}

float Heatpad::getMeasuredCurrent() const {
    return mCurrentSenseConditioner.getFilteredValue();
}

} // namespace