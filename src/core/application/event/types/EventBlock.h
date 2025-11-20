#pragma once

#include "core/application/event/types/EventHeader.h"

namespace Garbox {

template<typename EventPayload>
struct EventBlock {
    EventHeader header;
    EventPayload payload;
};

} // namespace
