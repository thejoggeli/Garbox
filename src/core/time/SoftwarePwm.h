#pragma once

#include <functional>
#include "core/time/SoftwareTimer.h"
#include "TimeLiterals.h"

namespace Garbox {

/**
 * Software-based PWM generator intended for low-frequency control tasks such as heater or fan modulation.
 * - Produces a periodic on/off signal
 * - The duty cycle defines the high-time fraction of each period. 
 * - The mode determines whether the cycle starts with the high or low phase. 
 */
class SoftwarePwm {
public:

    enum class State : uint8_t {
        Reset,
        High,
        Low,
    };
    
    enum class Mode : uint8_t {
        StartHigh,
        StartLow,
    };

    using Handler = std::function<void(State state)>;

    SoftwarePwm(float duty, uint32_t periodMicros);

    void start();
    void stop();
    void tick();

    void setDutyCycle(float duty, bool finishCurrent = true); // duty value must be in the range [0, 1] 
    void setPeriodDurationMicros(uint32_t durationMicros, bool finishCurrent = true);
    void setMode(Mode hase);
    void setStateChangedHandler(Handler handler);

    float getCurrentDutyCycle() const;
    float getNextDutyCycle() const;
    uint32_t getCurrentPeriodDurationMicros() const;
    uint32_t getNextPeriodDurationMicros() const;

    uint32_t getProgressMicros() const;
    float getProgressRelative() const;

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
    Mode mMode = Mode::StartLow;
    Handler mHandler = nullptr;

    bool mFirstTick = true;

};

} // namespace