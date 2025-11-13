#pragma once

#include "core/hardware/timer/Timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

namespace Garbox {

class Gpio;

class FrequencySensor {
public:

    struct Config {
        uint32_t stopTimeoutMicros = 100'000; // default 100 ms -> 10 Hz timeout
    };

    FrequencySensor(Gpio& gpio, Timer& timer);

    void init(Config const& config);
    void tick();

    void setEnabled(bool enabled);
    bool isEnabled();

    float getFrequencyHz();

    // Disallow copy and move 
    FrequencySensor(const FrequencySensor&) = delete;
    FrequencySensor& operator=(const FrequencySensor&) = delete;
    FrequencySensor(FrequencySensor&&) = delete;
    FrequencySensor& operator=(FrequencySensor&&) = delete;

private:

    enum class State : uint8_t {
        Idle = 0,
        Running,
        Disabled 
    };

    static void IRAM_ATTR isrHandler(void* arg);

    Gpio& mGpio;
    Timer& mTimer;

    int32_t mPin = -1;

    // state handling
    bool mInitialized = false;
    State mState = State::Disabled;

    // ticks until timeout
    uint32_t mStopTimeoutTicks = 0;

    // timer tick frequency
    float mTimerFrequencyHz = 1;

    // measured frequency
    float mMeasuredFrequencyHz = 0;

    // used inside ISR
    volatile uint32_t vLastEdgeTicks = 0; // t[n-1]
    volatile uint32_t vCurrentEdgeTicks = 0; // t[n]
    volatile bool vHasNewEdge = false;
    portMUX_TYPE mFrequencySensorMux = portMUX_INITIALIZER_UNLOCKED;

};

} // namespace Garbox
