#pragma once

#include <cstdint>
#include "core/application/event/Event.h"

namespace Garbox {

template<typename EventType>
struct EventWrapper {
    Event* event;
    EventType* data;

    EventWrapper():
        event(nullptr),
        data(nullptr){
        // constructor body
    }

    EventWrapper(Event* eventPtr):
        event(eventPtr),
        data(eventPtr ? static_cast<EventType*>(eventPtr->data) : nullptr){
        // constructor body
    }

};

} // namespace
