#pragma once

#include <cstdint>
#include "core/event/Event.h"

namespace Garbox {

template<typename EventType>
struct EventView {
    const Event* event;
    const EventType* data;

    EventView(const Event* eventPtr):
        event(eventPtr),
        data(static_cast<EventType*>(eventPtr->data)){
        // constructor body
    }

};

} // namespace
