#pragma once

#include <cstdint>

namespace Garbox {

enum class EventType : uint16_t {
    Null = 0,
    Fan,
    Button,
};

} // namespace
