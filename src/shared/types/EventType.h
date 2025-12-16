#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>

namespace Garbox {

enum class EventType : uint16_t {

    // null event
    Null = 0,

    // generated from config
    Heartbeat,
    DisplayCommand,
    FanCommand,
    HeatpadCommand,
    ButtonStateChanged,
    ButtonRepeat,
    EncoderStep,
    FermentationStatus,
    DisplayStatus,
    FanStatus,
    FanSample,
    HeatpadStatus,
    HeatpadSample,
    TemperatureStatus,
    TemperatureSample,
    ActiveBehaviourChanged,
    ActiveScreenChanged,

    // enum size marker
    Count,
};

const char* EventTypeToString(EventType type);

} // namespace Garbox