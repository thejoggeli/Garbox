#pragma once

#include <cstring>
#include "shared/types/EventType.h"
#include "core/application/event/Event.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"

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

struct HeatpadStatus {
    static constexpr EventType Type = EventType::HeatpadStatus;
    HeatpadState state = HeatpadState::Disabled;
    float duty = 0.0f;
    uint32_t periodMicros = 0;
};

struct TemperatureStatus {
    static constexpr EventType Type = EventType::TemperatureStatus;
    bool sensorEnabled = false;
    bool sensorError = false;
    float temperatureCelcius = 0.0f;
    float humidityRelative = 0.0f;
};

struct FanCommand {
    static constexpr EventType Type = EventType::FanCommand;
    bool enabled = false;
    float targetSpeed = 0.0f;
};

struct HeatpadCommand {
    static constexpr EventType Type = EventType::HeatpadCommand;
    bool enabled = false;
    float dutyCycle = 0.0f;
    uint32_t periodMicros = 0;
};

} // namespace
