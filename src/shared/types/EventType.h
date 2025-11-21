// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include <cstdint>



namespace Garbox {

enum class EventType : uint16_t {

    // null event
    Null = 0,

    Heartbeat,
    BacklightCommand,
    FanStatus,
    FanSample,
    FanCommand,
    HeatpadStatus,
    HeatpadCommand,
    TemperatureStatus,
    TemperatureSample,
    ButtonStateChanged,
    ButtonRepeat,
    EncoderStep,

    // enum size marker
    Count,
};

const char* EventTypeToString(EventType type);

} // namespace Garbox