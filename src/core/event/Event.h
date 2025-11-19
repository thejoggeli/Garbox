#pragma once

#include <cstdint>
#include "core/types/ComponentDescriptor.h"
#include "shared/types/EventType.h"

namespace Garbox {

struct Event {
    uint32_t id = 0; // zero is reserved for "invalid event id"
    EventType type = EventType::Null;
    ComponentDescriptor sender = {ComponentType::Null, ComponentId::Null};
    void* data = nullptr;
};

} // namespace
