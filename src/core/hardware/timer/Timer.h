#pragma once

#include "driver/timer.h"

namespace Garbox {

class Timer {

public:

    Timer(timer_group_t group, timer_idx_t index, uint32_t frequencyHz);

    void init(timer_config_t const& config);
    void resume();
    void pause();

    void setValue(uint32_t value);
    void setValue(uint64_t value);
    void setValueFromIsr(uint32_t value);
    void setValueFromIsr(uint64_t value);

    uint64_t      getValue();
    uint64_t      getValueFromIsr();
    timer_group_t getGroup();
    timer_idx_t   getIndex();
    uint32_t      getFrequencyHz();

private:

    timer_config_t mConfig;
    timer_group_t  mGroup;
    timer_idx_t    mIndex;
    uint32_t       mFrequencyHz;

    bool mRunning = false;
    bool mInitialized = false;

};

}