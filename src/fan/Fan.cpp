#include "Fan.h"

#include <Arduino.h>

#include "assert/Assert.h"
#include "config/GlobalConfig.h"
#include "config/GpioConfig.h"
#include "config/LedcPwmConfig.h"
#include "config/PcntConfig.h"
#include "core/Time.h"
#include "driver/pcnt.h"
#include "utils/MathUtils.h"

namespace Garbox {

Fan::Fan() : 
    // init members
    mGpioFanEnable(GpioConfig::FanEnable),
    mLedcPwm(GpioConfig::FanPwm, LedcPwmConfig::FanPwm, FanPwmFrequencyHz, FanPwmResolutionBits),
    mTachoPulseCounter(GpioConfig::FanTacho, PcntConfig::FanTachoUnit, PcntConfig::FanTachoChannel){
    // nothing to do
}

void Fan::init(){
    // init fan pwm
    mLedcPwm.init();
    mLedcPwm.setDutyNormalized(1.0f); // pwm pin logic is inverted

    // init fan enable
    mGpioFanEnable.setMode(Gpio::Mode::Output);
    mGpioFanEnable.digitalWrite(false);

    // init fan tacho
    mTachoPulseCounter.init();

    // configure exponential smoothing alpha to reach fraction in given amount of time
    constexpr float smoothingFraction = 0.99f;
    constexpr float smoothingSeconds =  1.0f;
    mSmoothingAlpha = MathUtils::computeAlpha(smoothingFraction, static_cast<uint32_t>(GlobalConfig::targetTickRateHz * smoothingSeconds));

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
    
    // compute smooth rpm value
    if(mSmoothRpmValue != mLastRpmValue){
        mSmoothRpmValueFloat = MathUtils::exponentialSmoothing(mSmoothRpmValueFloat, static_cast<float>(mLastRpmValue), mSmoothingAlpha);
        mSmoothRpmValue = static_cast<uint32_t>(mSmoothRpmValueFloat);
        if(mSmoothRpmValue == mLastRpmValue){
            mSmoothRpmValueFloat = static_cast<float>(mSmoothRpmValue);
        }
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
    mSpeed = MathUtils::clamp(speed, 0.0f, 1.0f);
    mLedcPwm.setDutyNormalized(1.0f - mSpeed); // pwm pin logic is inverted
}

float Fan::getSpeed(){
    return mSpeed;
}

void Fan::updateMeasuredRpm(){

	// get timestamp and tacho counter
    uint32_t currentTimeMicros = Time::GetMicros();
    int16_t tachoCount = mTachoPulseCounter.getAndClearCount();

    // tacho counter must not be negative
    if(tachoCount < 0){
        AssertDebug(false, "Fan tach cnt < 0");
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
    float const deltaTimeSeconds = static_cast<float>(deltaTimeMicros) * Time::MicrosToSeconds;

    // compute rpm
    constexpr float pulsesPerRevInv = 1.0F / static_cast<float>(PulsesPerRevolution);
    float const pulsesPerSecond = static_cast<float>(tachoCount) / deltaTimeSeconds;
    float const rpmFloat = pulsesPerSecond * pulsesPerRevInv * 60.0F;

    // update rpm
    mLastRpmValue = static_cast<uint32_t>(rpmFloat);
    mLastRpmTimeMicros = currentTimeMicros;
    mLastTachoCount = tachoCount;
}

uint32_t Fan::getMeasuredRpm(){
    return static_cast<uint32_t>(mSmoothRpmValue);
}

} // namespace