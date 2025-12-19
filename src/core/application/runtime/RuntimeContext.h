#pragma once

#include <cstdint>

namespace Garbox {

struct RuntimeContext {

    uint32_t tickCount = 0;
    uint32_t eventCount = 0;
    uint32_t stateChangesCount = 0;

};

} // namespace 
