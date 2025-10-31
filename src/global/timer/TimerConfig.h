#pragma once

#include "driver/timer.h"

namespace Garbox {

class TimerConfig {
public:

    struct Config {
        timer_group_t group;
        timer_idx_t idx;
        timer_config_t cfg;
        uint32_t frequencyHz;
        uint64_t maxValue = 0;
    };

    static constexpr Config FanTachoTimer {
        .group = TIMER_GROUP_0,
        .idx   = TIMER_0,
        .cfg   = {
            .alarm_en = TIMER_ALARM_DIS,
            .counter_en = TIMER_START,
            .intr_type = TIMER_INTR_LEVEL,
            .counter_dir = TIMER_COUNT_UP,
            .auto_reload = TIMER_AUTORELOAD_EN,
            .divider = 0, // computed from frequencyHz in Timer::init() 
            .clk_src = TIMER_SRC_CLK_APB
        },
        .frequencyHz = 10'000'000,
        .maxValue = 0,
    };

};

} // namespace