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
    FermentationStatus,
    DisplayCommand,
    DisplayStatus,
    FanStatus,
    FanSample,
    FanCommand,
    HeatpadStatus,
    HeatpadSample,
    HeatpadCommand,
    TemperatureStatus,
    TemperatureSample,
    ButtonStateChanged,
    ButtonRepeat,
    EncoderStep,
    ActiveBehaviourChanged,
    ActiveScreenChanged,

    // enum size marker
    Count,
};

const char* EventTypeToString(EventType type);

} // namespace Garbox