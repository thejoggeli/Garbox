#pragma once

#include <cstdint>

namespace Garbox {

enum class EventType : uint16_t {
    
    // null event
    Null = 0,

    // status events
    Heartbeat,
    FanStatus,
    HeatpadStatus,
    TemperatureStatus,

    // command events
    FanCommand,
    HeatpadCommand,

    // enum size 
    Count,
};

const char* EventTypeToString(EventType type);

} // namespace
