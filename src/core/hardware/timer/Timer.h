#pragma once

#include "driver/timer.h"

namespace Garbox {

class Timer {

public:

    struct InitStruct {
        timer_group_t group; 
        timer_idx_t index; 
        uint32_t frequencyHz;
        uint64_t maxValue;
        timer_config_t config;
    };

    Timer();

    void init(InitStruct const& initStruct);
    void start();
    void stop();
    void reset();

    void setValue(uint32_t value);
    void setValue(uint64_t value);
    void setValueFromIsr(uint32_t value);
    void setValueFromIsr(uint64_t value);

    uint64_t getValue();
    uint64_t getValueFromIsr();

    timer_group_t getGroup();
    timer_idx_t getIndex();
    uint32_t getFrequencyHz();
    uint64_t getMaxValue();

    // Disallow copy and move 
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer&&) = delete;

private:

    timer_config_t mConfig;
    timer_group_t mGroup;
    timer_idx_t mIndex;
    uint32_t mFrequencyHz;
    uint64_t mMaxValue;

    bool mRunning = false;
    bool mInitialized = false;

};

}