// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include <cstdint>

namespace Garbox {

enum class ControllerId : uint16_t {

    // null event
    Null = 0,

    Display,
    Fan,
    Heartbeat,
    Heatpad,
    Input,
    I2cParts,

    // enum size marker
    Count,
};

const char* ControllerIdToString(ControllerId id);

} // namespace Garbox