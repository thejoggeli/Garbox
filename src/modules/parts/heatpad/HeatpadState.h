#pragma once

#include <cstdint>

namespace Garbox {

enum class HeatpadState : uint8_t {
    Disabled = 0, // output OFF
    Heating,      // output ON and generating heat
    Idle,         // output ON but PWM currently low 
    Count
};

const char* HeatpadStateToString(HeatpadState state);

} // namespace
