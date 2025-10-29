#pragma once

#include <cstdint>
#include "core/hardware/mcpwm/McpwmPair.h"
#include "global/PinConfig.h" 

namespace Garbox {

class McpwmConfig {
public:

    // Piezo
    static constexpr McpwmPair::Unit PiezoUnit = McpwmPair::Unit::Unit0;;
    static constexpr McpwmPair::Timer PiezoTimer = McpwmPair::Timer::Timer0;
    static constexpr uint32_t PiezoPinA = PinConfig::PiezoA;
    static constexpr uint32_t PiezoPinB = PinConfig::PiezoB;
    static constexpr uint32_t PiezoInitialFrequency = 1000;
    static constexpr uint32_t PiezoInitialDutyPercent = 50.0f;

};

} // namespace