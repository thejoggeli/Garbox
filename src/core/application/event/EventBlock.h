#pragma once

#include "core/application/event/EventHeader.h"
#include "shared/types/EventType.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

template<EventType E>
struct EventBlock {
    EventHeader header;
    EventPayload<E> payload;
};

} // namespace
