#include "Fan.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"
#include "global/pcnt/PcntConfig.h"
#include "global/PinConfig.h"
#include "global/gpio/GpioInstances.h"
#include "global/ledc/LedcInstances.h"
#include "global/timer/TimerInstances.h"
#include "util/MathUtils.h"

namespace Garbox {

// RPM measure config 
static constexpr uint32_t PulsesPerRevolution = 2;

// Exponential filter config
static constexpr float RpmFilterFraction = 0.90f;
static constexpr uint32_t RpmFilterTicks = AppConfig::MainTaskFrequencyHz/2;
static constexpr float RpmFilterThreshold = 0.1f;

// Fan state monitor config
static constexpr uint32_t stallThreshold = 200_ms;
static constexpr uint32_t stalledCallbackPeriod = 500_ms;
static constexpr uint32_t minRpmThreshold = 50;
static constexpr uint32_t reenterStallCooldown = 0_ms;

Fan::Fan() : 
    // init members
    mGpioFanEnable(GpioInstances::GetFanEnable()),
    mSpeedPwm(LedcInstances::GetFanControlChannel()),
    mFrequencySensor(PinConfig::FanTacho, TimerInstances::GetFanTachoTimer()),
    mRpmFilter(RpmFilterFraction, RpmFilterTicks, RpmFilterThreshold),
    mFanStateMonitor(stallThreshold, stalledCallbackPeriod, minRpmThreshold, reenterStallCooldown){
    // nothing to do
}

void Fan::init(){
    // init tacho pulse counter
    FrequencySensor::Config config;
    config.pinMode = FrequencySensor::PinMode::Floating;
    config.stopTimeoutMicros = 500'000;
    mFrequencySensor.init(config);

    // init fan state monitor
    mFanStateMonitor.setCallback([this](FanStateMonitor::State state){
        this->handleFanStateMonitorCallback(state);
    });
}

void Fan::start(){
    // nothing to do
}

void Fan::tick(){

    if(isEnabled()){
        // measure tacho frequency
        mFrequencySensor.tick();
    
        // update measured rpm 
        float newFrequency = mFrequencySensor.getFrequencyHz();
        if(newFrequency != mMeasuredFrequency){
            mMeasuredFrequency = newFrequency;
            mMeasuredRpm = newFrequency * 60.0f / static_cast<float>(PulsesPerRevolution);
        }
    
        // filter rpm
        mRpmFilter.update(mMeasuredRpm);        
    }

}

void Fan::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void Fan::setEnabled(bool enabled){
    if(enabled == isEnabled()){
        return;
    }
    mGpioFanEnable.setValue(enabled);
    mFrequencySensor.setEnabled(enabled);
    if(!enabled){
        mState = State::Off;
        mMeasuredFrequency = 0;
        mMeasuredRpm = 0;
        mRpmFilter.setCurrentValue(0);
        mFanStateMonitor.reset();
    }
    else {
        mState = State::On;
    }
}


void Fan::setSpeed(float speed){
    mSpeed = MathUtils::Clamp<float>(speed, 0.0f, 1.0f);
    mSpeedPwm.setDutyRelative(mSpeed);
}

bool Fan::isEnabled(){
    return (mState != State::Off);
}

float Fan::getSpeed(){
    return mSpeed;
}

float Fan::getMeasuredRpm(bool filtered){
    if(filtered){
        return mRpmFilter.getCurrentValue();
    } 
    return mMeasuredRpm;
}

void Fan::handleFanStateMonitorCallback(FanStateMonitor::State state){
    // determine new fan state
    State newState;
    switch(state){
    case FanStateMonitor::State::Off:
        newState = State::Off;
        break;
    case FanStateMonitor::State::On:
        newState = State::On;
        break;
    case FanStateMonitor::State::Stalled:
        newState = State::Stalled;
        break;
    default:
        FailDebug("Fan", "unhandled fan monitor state");
        return;
    }

    // check if fan state is different
    if(newState == mState){
        return;
    }
    mState = newState;

    // execute callback
    if(mStateChangedCallback){
        mStateChangedCallback(mState);
    }
}

} // namespace