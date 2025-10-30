#pragma once

#include "driver/gpio.h"
#include "driver/timer.h"

namespace Garbox {

class FrequencySensor {
public:

    enum class PinMode : uint8_t {
        Pullup,   // internal pull-up enabled
        Pulldown, // internal pull-down enabled
        Floating, // no internal pull
    };

    struct Config {
        PinMode pinMode = PinMode::Floating;
        uint32_t stopTimeoutMicros = 100'000; // default 100 ms -> 10 Hz timeout
        uint32_t timerFrequencyHz = 1'000'000; // default 1 MHz -> 1 µs per tick
    };

    FrequencySensor(uint32_t pin, timer_group_t timerGroup, timer_idx_t timerId);

    bool init(Config const& config);
    void tick();
    void setEnabled(bool enabled);
    float getFrequencyHz();


private:

    enum class State : uint8_t {
        Idle = 0,
        Running,
    };

    static void IRAM_ATTR isrHandler(void* arg);

    uint32_t mPin;
    timer_group_t mTimerGroup;
    timer_idx_t mTimerId;

    uint32_t mStopTimeoutTicks = 0;
    float mTimerFrequencyHz = 1;
    volatile State mState = State::Idle;
    volatile uint32_t mLastEdgeTicks = 0; // t[n-1]
    volatile uint32_t mCurrentEdgeTicks = 0; // t[n]
    volatile bool mHasNewEdge = false;
    float mFrequencyHz = 0;
    bool mInitialized = false;
    bool mEnabled = false;

};

} // namespace Garbox
