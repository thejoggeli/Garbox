#pragma once

#include <cstring>
#include "app/types/EventType.h"
#include "core/event/Event.h"
#include "parts/fan/FanState.h"
#include "parts/heatpad/HeatpadState.h"

namespace Garbox::EventData {

struct Heartbeat {
    static constexpr EventType Type = EventType::Heartbeat;
};

struct FanStatus {
    static constexpr EventType Type = EventType::FanStatus;
    FanState state = FanState::Disabled;
    float targetSpeed = 0.0f;
    float measuredRpm = 0.0f;
};

struct FanCommand {
    static constexpr EventType Type = EventType::FanCommand;
    bool enabled = false;
    float targetSpeed = 0.0f;
};

struct HeatpadStatus {
    static constexpr EventType Type = EventType::HeatpadStatus;
    HeatpadState state = HeatpadState::Disabled;
};

struct HeatpadCommand {
    static constexpr EventType Type = EventType::HeatpadCommand;
    bool enabled = false;
    float dutyCycle = 0.0f;
    uint32_t periodMicros = 0;
};

} // namespace
