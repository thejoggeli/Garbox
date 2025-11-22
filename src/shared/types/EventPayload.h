// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include <cstdint>
#include <cstring>
#include "shared/types/EventType.h"

// included from config
#include "modules/parts/button/ButtonState.h"
#include "modules/parts/heatpad/HeatpadState.h"
#include "modules/parts/fan/FanState.h"

namespace Garbox::EventPayload {

struct Heartbeat {
    static constexpr EventType Type = EventType::Heartbeat;
};

struct BacklightCommand {
    static constexpr EventType Type = EventType::BacklightCommand;
    float brightness;          
};

struct FanStatus {
    static constexpr EventType Type = EventType::FanStatus;
    FanState state;          
    float targetSpeed;          
};

struct FanSample {
    static constexpr EventType Type = EventType::FanSample;
    float measuredRpm;          
};

struct FanCommand {
    static constexpr EventType Type = EventType::FanCommand;
    bool enabled;          
    float targetSpeed;          
};

struct HeatpadStatus {
    static constexpr EventType Type = EventType::HeatpadStatus;
    HeatpadState state;          
    float dutyCycle;          
    uint32_t periodMicros;          
};

struct HeatpadCommand {
    static constexpr EventType Type = EventType::HeatpadCommand;
    bool enabled;          
    float dutyCycle;          
    uint32_t periodMicros;          
};

struct TemperatureStatus {
    static constexpr EventType Type = EventType::TemperatureStatus;
    bool driverEnabled;          
    bool powerEnabled;          
    bool resetting;          
};

struct TemperatureSample {
    static constexpr EventType Type = EventType::TemperatureSample;
    float temperatureCelcius;          
    float humidityRelative;          
};

struct ButtonStateChanged {
    static constexpr EventType Type = EventType::ButtonStateChanged;
    ButtonState oldState;          
    ButtonState newState;          
};

struct ButtonRepeat {
    static constexpr EventType Type = EventType::ButtonRepeat;
    uint32_t count;          
};

struct EncoderStep {
    static constexpr EventType Type = EventType::EncoderStep;
    int32_t steps;          
};

} // namespace Garbox::EventPayload