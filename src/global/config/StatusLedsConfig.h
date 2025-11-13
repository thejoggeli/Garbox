#pragma once

namespace Garbox {

enum class StatusLed : uint8_t {
    Heartbeat = 0, 
    Custom1 = 1,
    Custom2 = 2,
    Error = 3,
};

} // namespace