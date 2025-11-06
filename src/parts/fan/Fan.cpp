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

Fan::Fan() : 
    // init members
    mGpioFanEnable(GpioInstances::GetFanEnable()),
    mSpeedPwm(LedcInstances::GetFanControlChannel()),
    mFrequencySensor(PinConfig::FanTacho, TimerInstances::GetFanTachoTimer()),
    mRpmFilter(),
    mMonitor(){
    // nothing to do
}

void Fan::init(){
    // init frequency sensor
    FrequencySensor::Config config;
    config.pinMode = FrequencySensor::PinMode::Floating;
    config.stopTimeoutMicros = 1000_ms;

    // init frequency sensor
    mFrequencySensor.init(config);

    // init monitor
    mMonitor.init();
    mMonitor.setMinRpmThreshold(100);
    mMonitor.setStalledAlertPeriod(1000_ms);

    // set monitor transition delays
    mMonitor.setTransitionDelay(MonitorState::Idle,     MonitorState::Stalled,  500_ms); // detect fan not starting 
    mMonitor.setTransitionDelay(MonitorState::Idle,     MonitorState::Spinning, 100_ms); // debounce Idle => Spinning
    mMonitor.setTransitionDelay(MonitorState::Spinning, MonitorState::Idle,     0_ms);   // already large delay on rpmValue=0 from FrequencySensor
    mMonitor.setTransitionDelay(MonitorState::Spinning, MonitorState::Stalled,  0_ms);   // already large delay on rpmValue=0 from FrequencySensor
    mMonitor.setTransitionDelay(MonitorState::Stalled,  MonitorState::Idle,     0_ms);   // -
    mMonitor.setTransitionDelay(MonitorState::Stalled,  MonitorState::Spinning, 100_ms); // debounce Stalled => Spinning

    // set monitor callbacks
    mMonitor.setStateChangedCallback([this](MonitorState oldState, MonitorState newState){
        this->handleMonitorStateChanged(oldState, newState);
    });
    mMonitor.setStalledAlertCallback([this](uint32_t counter){
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
            mMeasuredRpm = static_cast<uint32_t>(newFrequency * 60.0f / static_cast<float>(PulsesPerRevolution));
        }
    }

    // filter rpm
    mRpmFilter.add(mMeasuredRpm);   
    mMeasuredRpmFiltered = mRpmFilter.getAverage();

    // fan monitor tick
    const bool shouldSpin = isEnabled();
    const uint32_t unfilteredRpm = mMeasuredRpm;
    mMonitor.tick(unfilteredRpm, shouldSpin);
}

void Fan::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void Fan::setStalledAlertCallback(StalledAlertCallback callback){
    mStalledAlertCallback = callback;
}

void Fan::setSpeed(float speed) {
    mSpeed = MathUtils::Clamp<float>(speed, 0.0f, 1.0f);
    mSpeedPwm.setDutyRelative(mSpeed);
}

void Fan::setEnabled(bool enabled){
    if(enabled == mEnabled){
        return;
    }
    mEnabled = enabled;
    if(enabled){
        mGpioFanEnable.setValue(true);
        mFrequencySensor.setEnabled(true);
    }
    else {
        mMeasuredFrequency = 0;
        mMeasuredRpm = 0;
        mGpioFanEnable.setValue(false);
        mFrequencySensor.setEnabled(false);
    }
    updateState();
}

void Fan::updateState(){
    const State oldState = mState;
    if(mEnabled){
        if(mMonitor.getState() == MonitorState::Stalled){
            mState = State::Stalled;
        }
        else {
            mState = State::Enabled;
        }
    }
    else {
        mState = State::Disabled;
    }
    if((mState != oldState) && mStateChangedCallback){
        mStateChangedCallback(oldState, mState);
    }
}

void Fan::handleMonitorStateChanged(MonitorState oldState, MonitorState newState){
    updateState();
}

void Fan::handleMonitorStalledAlert(uint32_t counter){
    if(mStalledAlertCallback){
        mStalledAlertCallback(counter);
    }
}

bool Fan::isEnabled() const {
    return mEnabled;
}

bool Fan::isStalled() const {
    return (mState == State::Stalled); 
}

Fan::State Fan::getState() const {
    return mState;
}

float Fan::getSpeed() const {
    return mSpeed;
}

uint32_t Fan::getMeasuredRpm(bool filtered) const {
    if(filtered){
        return mMeasuredRpmFiltered;
    } 
    return mMeasuredRpm;
}

const char* Fan::StateToString(State state){
    switch(state){
    case State::Disabled: return "Disabled";
    case State::Enabled:  return "Enabled";
    case State::Stalled:  return "Stalled";
    case State::Count:  return "Count";
    }
    return "Unknown";
}

} // namespace