#pragma once

#include <cstring>
#include "EventType.h"

namespace Garbox {

struct FanEvent {
    static constexpr uint16_t Type = static_cast<uint16_t>(EventType::Fan);
    float measuredRpm;
    float targetSpeed;
};

struct ButtonEvent {
    static constexpr uint16_t Type = static_cast<uint16_t>(EventType::Button);
    bool pressed;
};

} // namespace
