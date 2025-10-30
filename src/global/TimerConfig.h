#pragma once

#include "driver/timer.h"

namespace Garbox {

class TimerConfig {
public:

    static constexpr timer_group_t FanTachoGroup = TIMER_GROUP_0;
    static constexpr timer_idx_t   FanTachoTimer = TIMER_0;

};

} // namespace