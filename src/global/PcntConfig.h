#pragma once

#include <cstdint>
#include "driver/pcnt.h"
#include "global/PinConfig.h"

namespace Garbox {

class PcntConfig {
public:

    struct Config {
        pcnt_channel_t channel;
        pcnt_unit_t unit;
        uint32_t pin;
    };

    static constexpr Config FanTacho       { PCNT_CHANNEL_0, PCNT_UNIT_0, PinConfig::FanTacho}; 
    static constexpr Config RotaryEncoderA { PCNT_CHANNEL_0, PCNT_UNIT_1, PinConfig::RotaryEncoderA}; 
    static constexpr Config RotaryEncoderB { PCNT_CHANNEL_0, PCNT_UNIT_2, PinConfig::RotaryEncoderB}; 
    static constexpr Config RotaryEncoderC { PCNT_CHANNEL_0, PCNT_UNIT_3, PinConfig::RotaryEncoderC}; 

};

} // namespace