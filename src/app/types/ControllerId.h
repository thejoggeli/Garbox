#pragma once

#include <cstdint>

namespace Garbox {

enum class ControllerId : uint8_t {
    Null = 0,
    Fan,
    Heartbeat,
    Garbox,
    Heatpad,
    Display,
};

} // namespace