#include "Fan.h"

#include <Arduino.h>

#include "assert/Assert.h"
#include "config/GpioConfig.h"
#include "config/PcntConfig.h"
#include "core/Time.h"
#include "driver/pcnt.h"
#include "utils/MathUtils.h"

namespace Garbox {

Fan::Fan() : 
    // init members
    mTachoPulseCounter(GpioConfig::FAN_TACHO, PcntConfig::FAN_TACHO_UNIT, PcntConfig::FAN_TACHO_CHANNEL),
    mGpioFanEnable(GpioConfig::FAN_ENABLE),
    mGpioFanPwm(GpioConfig::FAN_PWM){
    // nothing to do
}

void Fan::init(){
    if(!mTachoPulseCounter.init()){
        AssertExit(false, "Fan: failed to initialize tacho pulse counter");
    }
    mGpioFanEnable.setMode(Gpio::Mode::Output);
    mGpioFanPwm.setMode(Gpio::Mode::Output);
    setEnabled(0);
    setSpeed(0.0F);
}

void Fan::start(){
    mTachoPulseCounter.start();
	mLastRpmTimeMicros = Time::GetMicros();
}

void Fan::tick(){
    // update RPM
    if((Time::GetMicros() - mLastRpmTimeMicros) > RPM_INTERVAL_MICROS){
        updateRpm();
    }
}

void Fan::setEnabled(bool enabled){
    mEnabled = enabled;
    mGpioFanEnable.digitalWrite(mEnabled);
}
void Fan::setSpeed(float speed){
    mSpeed = MathUtils::clamp(speed, 0.0F, 1.0F);
    // pwm pin logic is inverted
    if(mSpeed > 0.5F){
        mGpioFanPwm.digitalWrite(0);
    } 
    else {
        mGpioFanPwm.digitalWrite(1);
    }
}

bool Fan::isEnabled(){
    return mEnabled;
}
float Fan::getSpeed(){
    return mSpeed;
}

void Fan::updateRpm(){
	// get timestamp and tacho counter
    uint32_t currentTimeMicros = Time::GetMicros();
    int16_t tachoCounter = mTachoPulseCounter.getAndClearCount();

    // tacho counter must not be negative
    if(tachoCounter < 0){
        AssertDebug(false, "Fan tach cnt < 0");
        mLastRpmValue = 0;
        mLastRpmTimeMicros = currentTimeMicros;
        return;
    } 

    // compute delta time
    uint32_t const deltaTimeMicros = currentTimeMicros - mLastRpmTimeMicros;
    float const deltaTimeSeconds = static_cast<float>(deltaTimeMicros) * Time::MICROS_TO_SECONDS;

    // compute rpm
    constexpr float pulsesPerRevInv = 1.0F / static_cast<float>(PULSES_PER_REVOLUTION);
    float const pulsesPerSecond = static_cast<float>(tachoCounter) / deltaTimeSeconds;
    float const rpmFloat = pulsesPerSecond * pulsesPerRevInv * 60.0F;

    // update rpm
    mLastRpmValue = static_cast<uint32_t>(rpmFloat);
    mLastRpmTimeMicros = currentTimeMicros;
}

uint32_t Fan::getMeasuredRpm(){
    return mLastRpmValue;
}

} // namespace