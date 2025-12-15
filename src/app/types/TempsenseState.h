#pragma once

#include <cstdint>

namespace Garbox {

enum class TempsenseState : uint8_t {
    Initial = 0,    // initial state after startup
    Setup,          // setup in progress
    Running,        // sensor is doing stuff
    Reset,          // reset in progress
    Error,          // sensor has error
    Count
};

const char* TempsenseStateToString(TempsenseState state);

} // namespace
