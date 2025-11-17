#pragma once

#include <cstring>
#include "app/types/EventType.h"
#include "core/event/Event.h"

namespace Garbox {

struct FanEventData {
    static constexpr EventType Type = EventType::Fan;
    float measuredRpm;
    float targetSpeed;
};

struct ButtonEventData {
    static constexpr EventType Type = EventType::Button;
    bool pressed;
};

struct HeartbeatEventData {
    static constexpr EventType Type = EventType::Heartbeat;
};

} // namespace
