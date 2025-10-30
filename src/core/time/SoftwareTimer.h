#pragma once

#include <cstdint>
#include "TimeLiterals.h"

namespace Garbox {

/**
 * Uses microseconds for maximum performance on esp32
 */
class SoftwareTimer {
public:

    SoftwareTimer();
    
    /// start the timer
    void start(uint32_t durationMicros);

    /// reset the timer
    void reset();

    /// seamless restart with duration from last start
    void restart();

    /// seamless restart with new duration
    void restart(uint32_t newdurationMicros);

    /// extend the timer by duration 
    void extend(uint32_t durationMicros);

    /// returns true if timer ran past its duration
    bool isExpired() const;

    /// returns true if timer is in reset state
    bool isReset() const;

    /// returns true if timer is in running state
    bool isRunning() const;

    /// returns the elapsed time since the timer started
    uint32_t getElapsedMicros() const;

    /// returns the elapsed time as a fraction since the timer started
    float getElapsedFraction() const;

private:

    enum class State : uint8_t {
        Reset = 0,
        Running = 1,
    };

    uint32_t mStartTimeMicros = 0;
    uint32_t mDurationMicros = 0;
    State mState = State::Reset;
    
};

} // namespace