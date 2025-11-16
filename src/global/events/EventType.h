#pragma once

#include <cstdint>

namespace Garbox {

enum class EventType : uint16_t {
    Fan = 0,
    Button,
};

} // namespace
