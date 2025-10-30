#include "Fan.h"

#include "assert/Assert.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"
#include "global/PinConfig.h"
#include "global/ledc/LedcInstances.h"
#include "util/MathUtils.h"

namespace Garbox {

// RPM measure config 
static constexpr uint32_t RpmIntervalMicros = 1000 * 1000 / 5; // 5 Hz
static constexpr uint32_t PulsesPerRevolution = 2;

// Exponential filter config
static constexpr float RpmFilterFraction = 0.98f;
static constexpr uint32_t RpmFilterTicks = AppConfig::targetTickRateHz/2;
static constexpr float RpmFilterThreshold = 0.5f;

Fan::Fan() : 
    // init members
    mSpeedPwm(LedcInstances::GetFanControlChannel()),
    mTachoPulseCounter(PinConfig::FanTacho),
    mRpmFilter(RpmFilterFraction, RpmFilterTicks, RpmFilterThreshold){
    // nothing to do
}

void Fan::init(){

    // init fan enable
    mGpioFanEnable.setup(PinConfig::FanEnable, Gpio::Mode::Output);
    mGpioFanEnable.setValue(0);

    // init tacho pulse counter
    PulseCounter::Config pulseCounterConfig = {
        .pinMode = PulseCounter::PinMode::Floating,
        .glitchFilterNanos = 1000,
    };
    mTachoPulseCounter.init(pulseCounterConfig);

}

void Fan::start(){
    mTachoPulseCounter.start();
	mLastRpmTimeMicros = Time::GetMicros();
}

void Fan::tick(){
    // update RPM
    if((Time::GetMicros() - mLastRpmTimeMicros) > RpmIntervalMicros){
        updateMeasuredRpm();
    }
    
    // update filtered rpm value
    mRpmFilter.update(mLastRpmValue);
}
  
void Fan::setEnabled(bool enabled){
    if(mEnabled != enabled){
        mEnabled = enabled;
        mGpioFanEnable.setValue(mEnabled);
    }
}

bool Fan::isEnabled(){
    return mEnabled;
}

void Fan::setSpeed(float speed){
    mSpeed = MathUtils::clamp<float>(speed, 0.0f, 1.0f);
    mSpeedPwm.setDutyRelative(mSpeed);
}

float Fan::getSpeed(){
    return mSpeed;
}

void Fan::updateMeasuredRpm(){

	// get timestamp and tacho counter
    uint32_t currentTimeMicros = Time::GetMicros();
    int32_t tachoCount = mTachoPulseCounter.getAndClearCount();

    // tacho counter must not be negative
    if(tachoCount < 0){
        AssertDebug(false, "Fan::updateMeasuredRpm()", "tach cnt < 0");
        mLastRpmValue = 0;
        mLastRpmTimeMicros = currentTimeMicros;
        mLastTachoCount = 0;
        return;
    }

    // ignore +/-1 jumps => this always chooses higher stable value
    if((mLastTachoCount - 1) == tachoCount){
        mLastRpmTimeMicros = currentTimeMicros;
        return;
    }

    // compute delta time
    uint32_t const deltaTimeMicros = currentTimeMicros - mLastRpmTimeMicros;
    float const deltaTimeSeconds = static_cast<float>(deltaTimeMicros) * 1e-6f;

    // compute rpm
    constexpr float pulsesPerRevInv = 1.0F / static_cast<float>(PulsesPerRevolution);
    float const pulsesPerSecond = static_cast<float>(tachoCount) / deltaTimeSeconds;
    float const rpmFloat = pulsesPerSecond * pulsesPerRevInv * 60.0f;

    // update rpm
    mLastRpmValue = static_cast<uint32_t>(rpmFloat);
    mLastRpmTimeMicros = currentTimeMicros;
    mLastTachoCount = tachoCount;
}

uint32_t Fan::getMeasuredRpm(){
    return mRpmFilter.getCurrentValue();
}

} // namespace