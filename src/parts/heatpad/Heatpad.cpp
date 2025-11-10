#include "Heatpad.h"

#include <cmath>
#include "assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"
#include "global/hardware/adc/AdcInstances.h"
#include "global/hardware/gpio/GpioInstances.h"
#include "util/math/MathUtils.h"

namespace Garbox {

static constexpr float AdcFilterFration = 0.90f;
static constexpr uint32_t AdcFilterTicks = AppConfig::MainTaskFrequencyHz/2;
static constexpr uint32_t AdcFilterThreshold = 0.01f;

static constexpr uint32_t InitialPwmDuty = 0.5f;
static constexpr uint32_t InitialPwmPeriodMicros = 5'000'000; // 5 seconds

Heatpad::Heatpad() : 
    // init members
    mGpioHeatpadEnable(GpioInstances::GetHeatEnable()),
    mVoltageSenseAdc(AdcInstances::GetHeatpadVoltage()),
    mCurrentSenseAdc(AdcInstances::GetHeatpadCurrent()),
    mPwm(InitialPwmDuty, InitialPwmPeriodMicros){
    // nothing to do
}

void Heatpad::init(){

    // init adc filters
    mVoltageSenseFilter.setAlphaComputed(AdcFilterFration, AdcFilterTicks);
    mCurrentSenseFilter.setAlphaComputed(AdcFilterFration, AdcFilterTicks);
    mVoltageSenseFilter.setThreshold(AdcFilterThreshold);
    mCurrentSenseFilter.setThreshold(AdcFilterThreshold);
    
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

    // start log timer
    mLogTimer.start(250_ms);
}

void Heatpad::tick(){
    mPwm.tick();

    // sample adc voltages
    mVoltageSenseAdc.sample();
    mCurrentSenseAdc.sample();

    mVoltageSenseFilter.update(mVoltageSenseAdc.getVolts());
    mCurrentSenseFilter.update(mCurrentSenseAdc.getVolts());

    // log adc voltages
    if(mLogTimer.isExpired()){
        
        // get most recent adc voltages
        const float voltageSenseVolts = mVoltageSenseFilter.getFilteredValue();
        const float currentSenseVolts = mCurrentSenseFilter.getFilteredValue();

        // check if voltages changed since last log
        static float prevVoltageSenseVolts = 0.0f;
        static float prevCurrentSenseVolts = 0.0f;
        
        // compute voltage errors
        const float voltageSensorError = prevVoltageSenseVolts - voltageSenseVolts;
        const float currentSensorError = prevCurrentSenseVolts - currentSenseVolts;

        // integrate errors
        static float voltageSenseErrorIntegrated = 0.0f;
        static float currentSenseErrorIntegrated = 0.0f;
        voltageSenseErrorIntegrated += voltageSensorError;
        currentSenseErrorIntegrated += currentSensorError;
        
        // check if voltage changed by enough or large integrated error accumulated
        bool voltageSenseChanged = (std::fabs(voltageSenseErrorIntegrated) > 0.05f); 
        bool currentSenseChanged = (std::fabs(currentSenseErrorIntegrated) > 0.05f); 

        // print voltages if changed
        if(voltageSenseChanged || currentSenseChanged){
            prevVoltageSenseVolts = voltageSenseVolts;
            prevCurrentSenseVolts = currentSenseVolts;
            voltageSenseErrorIntegrated = 0.0f;
            currentSenseErrorIntegrated = 0.0f;
            LogDebug("Heatpad", "voltages usense=%5.3f V, isense=%5.3f V", voltageSenseVolts, currentSenseVolts);
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