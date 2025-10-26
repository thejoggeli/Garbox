#pragma once

#include <functional>
#include "core/SoftwareTimer.h"

namespace Garbox {

class SoftwarePwm {
public:

    enum class State : uint8_t {
        Off,
        High,
        Low,
    };

    using Handler = std::function<void(State state)>;

    SoftwarePwm(uint32_t periodMillis);

    void setStateChangedHandler(Handler handler);

    /// Turn off immediately
    void reset();

    void tick();

    /// Set duty cycle for future PWM intervals
    void setNextDutyCycle(float duty);

    float getCurrentDutyCycle();
    float getNextDutyCycle();

private:

    void enterState(State nextState);
    void startNextCycle();

    void updateDutyCycle(float duty);
    void runStateMachine();

    static constexpr uint32_t MaxRunsPerTick = 10;

    SoftwareTimer mPwmTimer;
    uint32_t mPeriodDuration;
    uint32_t mHighDuration;
    uint32_t mLowDuration;

    State mState = State::Off;
    Handler mHandler = nullptr;
    float mCurrentDutyCycle = 0.0f;
    float mNextDutyCycle = 0.0f;
    uint32_t mCurrentTickRunsCount = 0;

};

} // namespace