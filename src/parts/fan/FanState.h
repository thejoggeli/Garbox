#pragma once

#include <cstdint>

namespace Garbox {

enum class FanState : uint8_t {
    Disabled = 0,
    Enabled,
    Stalled,
    Count
};

const char* FanStateToString(FanState state);

} // namespace
