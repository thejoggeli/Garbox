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
    void setSpeed(float speed);

    bool isEnabled();
    float getSpeed();

    void updateRpm();
    uint32_t getMeasuredRpm();

private:

    static constexpr uint32_t RPM_INTERVAL_MICROS = 500 * 1000; // 2 Hz
    static constexpr uint32_t PULSES_PER_REVOLUTION = 2;

    uint32_t mLastRpmTimeMicros = 0;
    uint32_t mLastRpmValue = 0;

    bool mEnabled = 0;
    float mSpeed = 0.0F;

    TachoPulseCounter mTachoPulseCounter;
    Gpio mGpioFanEnable;
    Gpio mGpioFanPwm;

};

} // namespace