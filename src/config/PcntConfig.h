#pragma once

#include <cstdint>
#include "driver/pcnt.h"

namespace Garbox {

class PcntConfig {
public:

    // Fan
    static constexpr pcnt_unit_t FAN_TACHO_UNIT = PCNT_UNIT_0;
    static constexpr pcnt_channel_t FAN_TACHO_CHANNEL = PCNT_CHANNEL_0;

};

} // namespace