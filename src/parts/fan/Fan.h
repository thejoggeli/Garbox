#pragma once

#include <cstdint>

#include "core/hardware/gpio/Gpio.h" 
#include "core/sensor/FrequencySensor.h"
#include "util/filter/ExponentialFilter.h"

namespace Garbox {

class LedcChannel;

class Fan {
public:

    Fan();

    void init();
    void start();
    void tick();

    void setEnabled(bool enabled);
    void setSpeed(float speed); // range [0.0, 1.0]

    bool isEnabled();
    float getSpeed();
    float getMeasuredRpm(bool filtered = true);

private:

    float mMeasuredFrequency = 0.0f;
    float mMeasuredRpm = 0;

    bool mEnabled = false;
    float mSpeed = 0.0f;

    // sets voltage on FanEnable pin
    Gpio mGpioFanEnable;

    // sends pwm signal to FanPwm pin
    LedcChannel& mSpeedPwm;

    // counts tacho pulses on FanTacho pin
    FrequencySensor mFrequencySensor;

    // filter for measured RPM value
    ExponentialFilter mRpmFilter;

};

} // namespace