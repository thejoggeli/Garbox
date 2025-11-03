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
static constexpr uint32_t RpmFilterTicks = AppConfig::TargetTickRateHz/2;
static constexpr float RpmFilterThreshold = 0.1f;

Fan::Fan() : 
    // init members
    mGpioFanEnable(GpioInstances::GetFanEnable()),
    mSpeedPwm(LedcInstances::GetFanControlChannel()),
    mFrequencySensor(PinConfig::FanTacho, TimerInstances::GetFanTachoTimer()),
    mRpmFilter(RpmFilterFraction, RpmFilterTicks, RpmFilterThreshold){
    // nothing to do
}

void Fan::init(){
    // init tacho pulse counter
    FrequencySensor::Config config;
    config.pinMode = FrequencySensor::PinMode::Floating;
    config.stopTimeoutMicros = 500'000;
    mFrequencySensor.init(config);
}

void Fan::start(){
    // nothing to do
}

void Fan::tick(){

    if(mEnabled){
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
  
void Fan::setEnabled(bool enabled){
    if(mEnabled == enabled){
        return;
    }
    mEnabled = enabled;
    mGpioFanEnable.setValue(mEnabled);
    if(!enabled){
        mMeasuredFrequency = 0;
        mMeasuredRpm = 0;
        mRpmFilter.setCurrentValue(0);
    }
    mFrequencySensor.setEnabled(enabled);
}


void Fan::setSpeed(float speed){
    mSpeed = MathUtils::Clamp<float>(speed, 0.0f, 1.0f);
    mSpeedPwm.setDutyRelative(mSpeed);
}

bool Fan::isEnabled(){
    return mEnabled;
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

} // namespace