#pragma once

#include <cstdint>

namespace Garbox {

enum class ComponentType : uint8_t {
    Null = 0,
    Runtime,
    Controller,
    Behaviour,
    Count
};

const char* ComponentTypeToString(ComponentType id);

} // namespace