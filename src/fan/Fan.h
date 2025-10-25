#pragma once

#include <cstdint>

#include "core/Gpio.h" 
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

    static constexpr uint32_t RpmIntervalSeconds = 500 * 1000; // 2 Hz
    static constexpr uint32_t PulsesPerRevolution = 2;

    uint32_t mLastRpmTimeMicros = 0;
    uint32_t mLastRpmValue = 0;

    bool mEnabled = false;
    float mSpeed = 0.0F;

    TachoPulseCounter mTachoPulseCounter;
    Gpio mGpioFanEnable;
    Gpio mGpioFanPwm;

};

} // namespace