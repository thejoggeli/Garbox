#pragma once

#include <functional>
#include "core/time/SoftwareTimer.h"
#include "TimeLiterals.h"

namespace Garbox {

class SoftwarePwm {
public:

    enum class State : uint8_t {
        Reset,
        High,
        Low,
    };

    enum class Mode : uint8_t {
        HighLow,
        LowHigh,
    };

    using Handler = std::function<void(State state)>;

    SoftwarePwm(float duty, uint32_t periodMicros);

    void start();
    void stop();
    void tick();

    // duty value must be in the range [0, 1] 
    // duty=0 enters "Off" state
    void setDutyCycle(float duty, bool finishCurrent = true);
    void setPeriodDurationMicros(uint32_t durationMicros, bool finishCurrent = true);
    void setMode(Mode mode);
    void setStateChangedHandler(Handler handler);

    float getCurrentDutyCycle() const;
    float getNextDutyCycle() const;
    uint32_t getCurrentPeriodDurationMicros() const;
    uint32_t getNextPeriodDurationMicros() const;

    State getState() const;
    bool isReset() const;
    bool isRunning() const;

private:

    void startNextCycle();
    void updateState();
    void enterState(State state);
    void updateThreshold();

    SoftwareTimer mTimer;

    uint32_t mCurrentPeriodDurationMicros;
    uint32_t mNextPeriodDurationMicros;
    uint32_t mThresholdMicros = 0;

    float mCurrentDutyCycle;
    float mNextDutyCycle;

    State mState = State::Reset;
    Mode mMode = Mode::HighLow;
    Handler mHandler = nullptr;

};

} // namespace