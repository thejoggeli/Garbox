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
    mTachoPulseCounter.init();
    mGpioFanEnable.setMode(Gpio::Mode::Output);
    mGpioFanPwm.setMode(Gpio::Mode::Output);
    setEnabled(false);
    setSpeed(0.0F);
}

void Fan::start(){
    mTachoPulseCounter.start();
	mLastRpmTimeMicros = Time::GetMicros();
}

void Fan::tick(){
    // update RPM
    if((Time::GetMicros() - mLastRpmTimeMicros) > RpmIntervalSeconds){
        updateMeasuredRpm();
    }
}

void Fan::setEnabled(bool enabled){
    mEnabled = enabled;
    mGpioFanEnable.digitalWrite(mEnabled);
}

bool Fan::isEnabled(){
    return mEnabled;
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

float Fan::getSpeed(){
    return mSpeed;
}

void Fan::updateMeasuredRpm(){
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
    float const deltaTimeSeconds = static_cast<float>(deltaTimeMicros) * Time::MicrosToSeconds;

    // compute rpm
    constexpr float pulsesPerRevInv = 1.0F / static_cast<float>(PulsesPerRevolution);
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