#pragma once

#include <cstdint>

#include "core/Gpio.h" 
#include "core/SoftwareTimer.h"

namespace Garbox {

class Heatpad {
public:

    Heatpad();

    void init();
    void start();
    void tick();

    void setDutyCycle(float duty);
    void turnOff();

private:

    enum class State : uint8_t {
        Off,
        DutyHigh,
        DutyLow,
    };

    void setOutput(bool on);

    static constexpr uint32_t PwmTimerIntervalMillis = 5000; // 5 seconds

    State mState;

    Gpio mGpioHeatpadEnable;

    float mDutyCycle = 0.0f;
    uint32_t mHighDuration;
    uint32_t mLowDuration;
    uint32_t mFullDuration;
    SoftwareTimer mPwmTimer;

};

} // namespace