#pragma once

#include <cstdint>

namespace Garbox {

enum class ComponentType : uint8_t {
    Null = 0,
    Runtime,
    Replay,
    Controller,
    Behaviour,
    Screen,
    Count
};

const char* ComponentTypeToString(ComponentType id);

} // namespace