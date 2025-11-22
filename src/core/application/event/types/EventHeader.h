#pragma once

#include <cstdint>
#include "core/application/component/ComponentDescriptor.h"
#include "shared/types/EventType.h"

namespace Garbox {

struct EventHeader {

    // the id of the event. zero is reserved for "invalid event id"
    uint32_t id = 0;

    // event type from user-defined enum
    EventType type = EventType::Null;

    // component type + id of event sender
    ComponentDescriptor sender = {ComponentType::Null, ComponentId::Null};

};

} // namespace
