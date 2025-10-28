#pragma once

#include <functional>
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class SoftwarePwm {
public:

    enum class State : uint8_t {
        Reset,
        High,
        Low,
    };

    using Handler = std::function<void(State state)>;

    SoftwarePwm(uint32_t periodMillis);

    void setStateChangedHandler(Handler handler);

    /// Turn off immediately
    void reset();

    void tick();

    /// Duty value must be in the range [0, 1] 
    /// Duty=0 enters "Off" state
    void setDutyCycle(float duty, bool finishCurrent = true);

    float getCurrentDutyCycle();
    float getNextDutyCycle();

private:

    void runStateMachine();
    void enterState(State nextState);
    void startNextCycle();
    void applyDutyCycle(float duty);

    static constexpr uint32_t MaxRunsPerTick = 10;

    SoftwareTimer mPwmTimer;
    uint32_t mPeriodDuration;
    uint32_t mHighDuration;

    State mState = State::Reset;
    Handler mHandler = nullptr;
    float mCurrentDutyCycle = 0.0f;
    float mNextDutyCycle = 0.0f;
    uint32_t mCurrentTickRunsCount = 0;

};

} // namespace