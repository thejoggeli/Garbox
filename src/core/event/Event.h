#pragma once

#include <cstdint>
#include "app/types/ControllerId.h"
#include "app/types/EventType.h"

namespace Garbox {

struct Event {
    EventType type = EventType::Null;
    ControllerId sender = ControllerId::Null;
    uint8_t meta = 0;
    void* data = nullptr;
};

} // namespace
