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

    /// Set duty cycle for future PWM intervals
    void setNextDutyCycle(float duty);
    float getCurrentDutyCycle();
    float getNextDutyCycle();

    /// Turn off immediately
    void reset();

private:

    enum class State : uint8_t {
        Off,
        DutyHigh,
        DutyLow,
    };

    void setHeatEnabled(bool enabled);
    bool isHeatEnabled();

    void updatePwmDutyCycle(float duty);
    void updateStateMachine();

    void enterOffState();
    void enterPwmHighState();
    void enterPwmLowState();
    void enterNextPwmCycle();

    static constexpr uint32_t PwmTimerIntervalMillis = 5000; // 5 seconds

    Gpio mGpioHeatpadEnable;
    SoftwareTimer mPwmTimer;

    State mState = State::Off;
    bool mHeatEnabled = false;
    float mCurrentDutyCycle = 0.0f;
    float mNextDutyCycle = 0.0f;
    uint32_t mPwmHighDuration = 0; // heat on duration
    uint32_t mPwmLowDuration = PwmTimerIntervalMillis; // heat off duration

};

} // namespace