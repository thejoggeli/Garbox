#pragma once

#include <cstdint>
#include "app/types/ControllerId.h"
#include "app/types/EventType.h"

namespace Garbox {

struct Event {
    EventType type = EventType::Null;
    ControllerId sender = ControllerId::Null;
    void* data = nullptr;
};

} // namespace
