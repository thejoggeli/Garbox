#pragma once

#include <cstdint>

#include "core/Gpio.h" 
#include "core/LedcPwm.h"
#include "fan/TachoPulseCounter.h"
#include "filter/ExponentialFilter.h"

namespace Garbox {

class Fan {
public:

    Fan();

    void init();
    void start();
    void tick();

    void setEnabled(bool enabled);
    bool isEnabled();

    void setSpeed(float speed); // range [0.0, 1.0]
    float getSpeed();

    void updateMeasuredRpm();
    uint32_t getMeasuredRpm();

private:

    uint32_t mLastRpmTimeMicros = 0;
    uint32_t mLastRpmValue = 0;
    int16_t mLastTachoCount = 0;

    bool mEnabled = false;
    float mSpeed = 0.0f;

    // sets voltage on FanEnable pin
    Gpio mGpioFanEnable;

    // sends pwm signal to FanPwm pin
    LedcPwm mSpeedPwm;

    // counts tacho pulses on FanTacho pin
    TachoPulseCounter mTachoPulseCounter;

    // filter for measured RPM value
    ExponentialFilter<uint32_t> mRpmFilter;

};

} // namespace