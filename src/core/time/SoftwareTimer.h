#pragma once

#include <cstdint>

namespace Garbox {

class SoftwareTimer {
public:

    SoftwareTimer();
    
    /// start the timer
    void start(uint32_t durationMillis);

    /// reset the timer
    void reset();

    /// seamless restart with duration from last start
    void restart();

    /// seamless restart with new duration
    void restart(uint32_t newDurationMillis);

    /// extend the timer by duration 
    void extend(uint32_t durationMillis);

    /// returns true if timer ran past its duration
    bool isExpired() const;

    /// returns true if timer is in reset state
    bool isReset() const;

    // returns the elapsed time since the timer started
    uint32_t getElapsedMillis() const;

    // returns the elapsed time as a fraction since the timer started
    float getElapsedFraction() const;

private:

    uint32_t mStartTimeMillis = 0;
    uint32_t mDurationMillis = 0;

};

} // namespace