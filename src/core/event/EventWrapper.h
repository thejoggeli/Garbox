#pragma once

#include <cstdint>
#include "core/event/Event.h"

namespace Garbox {

template<typename EventType>
struct EventWrapper {
    Event* header;
    EventType* data;
};

} // namespace
