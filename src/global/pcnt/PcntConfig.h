#pragma once

#include <cstdint>
#include "driver/pcnt.h"
#include "global/PinConfig.h"

namespace Garbox {

class PcntConfig {
public:

    static constexpr pcnt_unit_t FanTachoUnit = PCNT_UNIT_0;
    static constexpr pcnt_unit_t RotaryEncoderUnit = PCNT_UNIT_1;

};

} // namespace