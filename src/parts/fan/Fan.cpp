#include "Fan.h"

#include "assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/hardware/ledc/LedcChannel.h"
#include "core/hardware/timer/Timer.h"
#include "core/time/Time.h"
#include "global/config/AppConfig.h"
#include "util/math/MathUtils.h"

namespace Garbox {

// RPM measure config 
static constexpr uint32_t PulsesPerRevolution = 2;

// RPM filter config
static constexpr size_t TachoConditionerWindowSize = std::max(1u, AppConfig::MainTaskFrequencyHz/3);

Fan::Fan(Gpio& gpioEnable, LedcChannel& speedPwm, Gpio& gpioTacho, Timer& timerTacho): 
    // init members
    mGpioFanEnable(gpioEnable),
    mSpeedPwm(speedPwm),
    mFrequencySensor(gpioTacho, timerTacho),
    mTachoConditioner(TachoConditionerWindowSize),
    mMonitor(){
    // nothing to do
}

void Fan::init(){
    AssertExit(!mInitialized, "Fan", "already initialized");

    // init frequency sensor
    FrequencySensor::Config config;
    config.stopTimeoutMicros = 1000_ms;

    // init rpm conditioner
    constexpr float HzToRpmFactor = 60.0f / static_cast<float>(PulsesPerRevolution);
    mTachoConditioner.setFixedPointScaling(1000.0f); // gives theoretical 0.001 RPM resolution
    mTachoConditioner.setInputScaling(HzToRpmFactor);
    mTachoConditioner.setOutputSnapping(10.0f, 0.5f); // snap to 10 RPM

    // init frequency sensor
    mFrequencySensor.init(config);

    // init monitor
    mMonitor.init();
    mMonitor.setMinRpmThreshold(100);
    mMonitor.setStalledAlertPeriod(1000_ms);

    // set monitor transition delays
    mMonitor.setTransitionDelay(MonitorState::Idle,     MonitorState::Stalled,  1000_ms); // detect fan not starting 
    mMonitor.setTransitionDelay(MonitorState::Idle,     MonitorState::Spinning, 100_ms) ; // debounce Idle => Spinning
    mMonitor.setTransitionDelay(MonitorState::Spinning, MonitorState::Idle,     0_ms);    // already large delay on rpmValue=0 from FrequencySensor
    mMonitor.setTransitionDelay(MonitorState::Spinning, MonitorState::Stalled,  0_ms);    // already large delay on rpmValue=0 from FrequencySensor
    mMonitor.setTransitionDelay(MonitorState::Stalled,  MonitorState::Idle,     0_ms);    // -
    mMonitor.setTransitionDelay(MonitorState::Stalled,  MonitorState::Spinning, 100_ms);  // debounce Stalled => Spinning

    // set monitor callbacks
    mMonitor.setStateChangedCallback([this](MonitorState oldState, MonitorState newState){
        this->handleMonitorStateChanged(oldState, newState);
    });
    mMonitor.setStalledAlertCallback([this](uint32_t counter){
        this->handleMonitorStalledAlert(counter);
    });

    // init complete
    mInitialized = true;
}

void Fan::start(){
    if(!mInitialized){
        TriggerDebug("Fan", "not initialized");
        return;
    }
}

void Fan::tick(){
    if(!mInitialized){
        TriggerDebug("Fan", "not initialized");
        return;
    }

    if(isEnabled()){
        // measure tacho frequency
        mFrequencySensor.tick();
    
        // get measured rpm and pass to conditioner 
        const float measuredFrequency = mFrequencySensor.getFrequencyHz();
        mTachoConditioner.process(measuredFrequency);
    }
    else {
        // filter rpm even if fan is not enabled for smooth transition to 0 RPM
        mTachoConditioner.process(0.0f);
    }

    // fan monitor tick
    const bool shouldSpin = isEnabled();
    const bool filtered = false;
    mMonitor.tick(getMeasuredRpm(filtered), shouldSpin);
}

void Fan::setStateChangedCallback(StateChangedCallback callback){
    mStateChangedCallback = callback;
}

void Fan::setStalledAlertCallback(StalledAlertCallback callback){
    mStalledAlertCallback = callback;
}

void Fan::setSpeed(float speed) {
    if(!mInitialized){
        TriggerDebug("Fan", "not initialized");
        return;
    }
    mSpeed = MathUtils::Clamp<float>(speed, 0.0f, 1.0f);
    mSpeedPwm.setDutyRelative(mSpeed);
}

void Fan::setEnabled(bool enabled){
    if(!mInitialized){
        TriggerDebug("Fan", "not initialized");
        return;
    }
    if(enabled == mEnabled){
        return;
    }
    mEnabled = enabled;
    if(enabled){
        mGpioFanEnable.writeLevel(true);
        mFrequencySensor.setEnabled(true);
    }
    else {
        mGpioFanEnable.writeLevel(false);
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

float Fan::getMeasuredRpm(bool filtered) const {
    if(filtered){
        return mTachoConditioner.getFilteredValue();
    }
    return mTachoConditioner.getUnfilteredValue();
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