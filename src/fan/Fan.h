#pragma once

#include <cstdint>

#include "core/Gpio.h" 
#include "core/LedcPwm.h"
#include "TachoPulseCounter.h"

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

    static constexpr uint32_t RpmIntervalMicros = 1000 * 1000 / 5; // 5 Hz
    static constexpr uint32_t PulsesPerRevolution = 2;
    static constexpr uint32_t FanPwmFrequencyHz = 25000;
    static constexpr uint32_t FanPwmResolutionBits = 8;

    uint32_t mLastRpmTimeMicros = 0;
    uint32_t mLastRpmValue = 0;
    int16_t mLastTachoCount = 0;

    uint32_t mSmoothRpmValue = 0;
    float mSmoothRpmValueFloat = 0.0f;
    float mSmoothingAlpha = 0.1f;

    bool mEnabled = false;
    float mSpeed = 0.0f;

    // sets voltage on FanEnable pin
    Gpio mGpioFanEnable;

    // sends pwm signal to FanPwm pin
    LedcPwm mLedcPwm;

    // counts tacho pulses on FanTacho pin
    TachoPulseCounter mTachoPulseCounter;

};

} // namespace