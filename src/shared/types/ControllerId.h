#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>

namespace Garbox {

enum class ControllerId : uint16_t {

    // null event
    Null = 0,

    // generated from config
    Display,
    Devtools,
    Fan,
    Heartbeat,
    Heatpad,
    Input,
    I2cParts,
    TimeSeries,

    // enum size marker
    Count,
};

const char* ControllerIdToString(ControllerId id);

} // namespace Garbox