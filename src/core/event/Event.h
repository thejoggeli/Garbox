#pragma once

#include <cstdint>
#include "shared/types/ControllerId.h"
#include "shared/types/EventType.h"

namespace Garbox {

struct Event {
    EventType type = EventType::Null;
    ControllerId sender = ControllerId::Null;
    void* data = nullptr;
};

} // namespace
