#pragma once

#include <cstdint>

namespace Garbox {

struct EventWrapper {
    uint16_t type;
    void* dataPtr;
};

} // namespace
