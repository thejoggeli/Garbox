#pragma once

#include <cstdint>

namespace Garbox {

enum class EventType : uint16_t {
    Null = 0,
    Heartbeat,
    FanStatus,
    FanCommand,
    HeatpadStatus,
    HeatpadCommand,
    Count,
};

const char* EventTypeToString(EventType type);

} // namespace
