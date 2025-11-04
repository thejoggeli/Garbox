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
static constexpr uint32_t RpmFilterTicks = AppConfig::MainTaskFrequencyHz/3;
static constexpr float RpmFilterThreshold = 0.05f;

// Fan state monitor config
static constexpr uint32_t StalledThreshold = 1000_ms;
static constexpr uint32_t StalledCallbackPeriod = 500_ms;
static constexpr uint32_t MinRpmThreshold = 10;
static constexpr uint32_t ReenterStallCooldown = 0_ms;

Fan::Fan() : 
    // init members
    mGpioFanEnable(GpioInstances::GetFanEnable()),
    mSpeedPwm(LedcInstances::GetFanControlChannel()),
    mFrequencySensor(PinConfig::FanTacho, TimerInstances::GetFanTachoTimer()),
    mRpmFilter(RpmFilterFraction, RpmFilterTicks, RpmFilterThreshold),
    mFanMonitor(StalledThreshold, StalledCallbackPeriod, MinRpmThreshold, ReenterStallCooldown){
    // nothing to do
}

void Fan::init(){
    // init frequency sensor
    FrequencySensor::Config config;
    config.pinMode = FrequencySensor::PinMode::Floating;
    config.stopTimeoutMicros = 1000_ms;

    // init frequency sensor
    mFrequencySensor.init(config);

    // init fan state monitor
    mFanMonitor.init();
    mFanMonitor.setStateChangedCallback([this](FanMonitor::State state){
        this->handleMonitorStateChanged(state);
    });
    mFanMonitor.setStalledAlertCallback([this](uint32_t counter){
        this->handleMonitorStalledAlert(counter);
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
        mMeasuredRpmFiltered = mRpmFilter.getCurrentValue();     
    }

    // fan monitor tick
    bool const shouldRun = isEnabled();
    uint32_t const unfilteredRpm = mMeasuredRpm;
    mFanMonitor.tick(unfilteredRpm, shouldRun);
}

void Fan::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void Fan::setStalledAlertCallback(StalledAlertCallback callback){
    mStalledAlertCallback = callback;
}

void Fan::setEnabled(bool enabled){
    if(enabled == isEnabled()){
        return;
    }
    mGpioFanEnable.setValue(enabled);
    mFrequencySensor.setEnabled(enabled);
    if(!enabled){
        enterState(State::Disabled);
    }
    else {
        enterState(State::Enabled);
    }
}

void Fan::setSpeed(float speed){
    mSpeed = MathUtils::Clamp<float>(speed, 0.0f, 1.0f);
    mSpeedPwm.setDutyRelative(mSpeed);
}

bool Fan::isEnabled(){
    return (mState != State::Disabled);
}

Fan::State Fan::getState(){
    return mState;
}

float Fan::getSpeed(){
    return mSpeed;
}

float Fan::getMeasuredRpm(bool filtered){
    if(filtered){
        return mMeasuredRpmFiltered;
    } 
    return mMeasuredRpm;
}

void Fan::enterState(State newState){
    State oldState = mState;
    if(oldState == newState){
        TriggerDebug("Fan", "already in state");
        return;
    }

    // handle state transition
    switch(newState){
    case State::Disabled:
        mMeasuredFrequency = 0;
        mMeasuredRpm = 0;
        mRpmFilter.setCurrentValue(0);
        break;
    case State::Enabled:
    case State::Stalled:
        // nothing to do
        break;
    default:
        TriggerDebug("Fan", "enter unhandled state");
        return;
    }
    mState = newState;

    // call state changed callback
    if(mStateChangedCallback){
        mStateChangedCallback(newState, oldState);
    }
}

void Fan::handleMonitorStateChanged(FanMonitor::State state){
    switch(state){
    case FanMonitor::State::Idle:
        break;
    case FanMonitor::State::Running:
        if(mState == State::Stalled){
            enterState(State::Enabled); // transition: Stalled -> Enabled
        }
        break;
    case FanMonitor::State::Stalled:
        if(mState == State::Enabled){ 
            enterState(State::Stalled); // transition: Stalled -> Stalled
        }
        break;
    default:
        TriggerDebug("Fan", "unhandled fan monitor state");
        return;
    }
}

void Fan::handleMonitorStalledAlert(uint32_t counter){
    if(mStalledAlertCallback){
        mStalledAlertCallback(counter);
    }
}

const char* Fan::StateToString(State state){
    switch(state){
    case State::Disabled: return "Disabled";
    case State::Enabled:  return "Enabled";
    case State::Stalled:  return "Stalled";
    }
    return "Invalid";
}

} // namespace