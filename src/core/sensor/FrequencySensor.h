#pragma once

#include "core/hardware/timer/Timer.h"

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
    };

    FrequencySensor(uint32_t pin, Timer& timer);

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
    Timer mTimer;

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
