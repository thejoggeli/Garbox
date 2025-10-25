#pragma once

#include <cstdint>

namespace Garbox {

class GpioConfig {
public:

    // Fan
    static constexpr uint32_t FAN_ENABLE = 4;
    static constexpr uint32_t FAN_TACHO = 5;
    static constexpr uint32_t FAN_PWM = 14;

};

} // namespace