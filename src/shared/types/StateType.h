#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>

namespace Garbox {

enum class StateType : uint16_t {

    // null state
    Null = 0,

    // generated from config
    DisplayStatus,
    DisplayDiagnostics,
    FanStatus,
    FanSample,
    FermentationStatus,
    HeatpadStatus,
    HeatpadSample,
    TemperatureStatus,
    TemperatureSample,
    ActiveBehaviour,
    ActiveScreen,

    // enum size marker
    Count,
};

const char* StateTypeToString(StateType type);

} // namespace Garbox