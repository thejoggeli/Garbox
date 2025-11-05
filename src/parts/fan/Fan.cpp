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
static constexpr uint32_t IdleStallThreshold = 1000_ms;
static constexpr uint32_t SpinningStallThreshold = 0_ms;
static constexpr uint32_t StalledCallbackPeriod = 500_ms;
static constexpr uint32_t MinRpmThreshold = 50;
static constexpr uint32_t ReenterStallCooldown = 0_ms;

Fan::Fan() : 
    // init members
    mGpioFanEnable(GpioInstances::GetFanEnable()),
    mSpeedPwm(LedcInstances::GetFanControlChannel()),
    mFrequencySensor(PinConfig::FanTacho, TimerInstances::GetFanTachoTimer()),
    mRpmFilter(RpmFilterFraction, RpmFilterTicks, RpmFilterThreshold),
    mFanMonitor(IdleStallThreshold, SpinningStallThreshold, StalledCallbackPeriod, MinRpmThreshold, ReenterStallCooldown){
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
    bool const shouldSpin = isEnabled();
    uint32_t const unfilteredRpm = mMeasuredRpm;
    mFanMonitor.tick(unfilteredRpm, shouldSpin);

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
    if(!enabled){
        enterState(State::Disabled);
    }
    else {
        enterState(State::Enabled);
    }
}

bool Fan::isEnabled(){
    return (mState != State::Disabled);
}

void Fan::enterState(State newState){
    State oldState = mState;
    if(oldState == newState){
        TriggerDebug("Fan", "already in state");
        return;
    }

    // handle state transition
    mState = newState;
    switch(mState){
    case State::Disabled:
        mMeasuredFrequency = 0;
        mMeasuredRpm = 0;
        mRpmFilter.setCurrentValue(0);
        mGpioFanEnable.setValue(false);
        mFrequencySensor.setEnabled(false);
        break;
    case State::Enabled:
        mGpioFanEnable.setValue(true);
        mFrequencySensor.setEnabled(true);
        break;
    case State::Stalled:
        break;
    default:
        TriggerDebug("Fan", "enter unhandled state");
        return;
    }

    // call state changed callback
    if(mStateChangedCallback){
        mStateChangedCallback(newState, oldState);
    }
}

void Fan::handleMonitorStateChanged(FanMonitor::State monitorState){
    if(monitorState == FanMonitor::State::Stalled){
        if(mState == State::Enabled){
            // enabled => stalled
            enterState(State::Stalled);
        }
        else if(mState == State::Stalled){
            TriggerDebug("Fan", "invalid spinning state change");
        }
    }
    else if(monitorState == FanMonitor::State::Spinning){
        if(mState == State::Stalled){
            // stalled => enabled
            enterState(State::Enabled);
        }
        else if(mState == State::Disabled){
            TriggerDebug("Fan", "invalid spinning state change");
        }
    }
    else if(monitorState == FanMonitor::State::Idle){
        if(mState != State::Disabled){
            TriggerDebug("Fan", "invalid idle state change");
        }
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

void Fan::setSpeed(float speed){
    mSpeed = MathUtils::Clamp<float>(speed, 0.0f, 1.0f);
    mSpeedPwm.setDutyRelative(mSpeed);
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

} // namespace