#pragma once

#include "core/application/event/EventBlock.h"
#include "core/application/event/EventHeader.h"
#include "core/assert/Assert.h"
#include "shared/types/EventType.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

/**
 * Lightweight, non-owning view of an event.
 * Holds pointers to header and payload; no data is copied.
 *
 * Always stores mutable pointers. Passing a const EventHeader* drops constness;
 * callers must ensure the underlying memory is safe to modify.
 *
 * - operator-> accessesthe payload
 * - header() returns the header pointer
 * - payload() returns the payload pointer
 * 
 * A runtime check verifies that the header’s event type matches E.
 */
template<EventType E>
class EventView {
public:

    EventView(EventHeader* header):
        mHeader(header),
        mPayload(EventView<E>::extractPayload(header)){
        // constructor body
    }

    EventView(const EventHeader* header):
        mHeader(const_cast<EventHeader*>(header)),
        mPayload(EventView<E>::extractPayload(mHeader)){
        // constructor body
    }

    // non-const access
    EventHeader* header() { return mHeader; }
    EventHeader* payload() { return mPayload; }
    EventPayload<E>* operator->() { return mPayload; }

    // const access
    const EventHeader* header() const { return mHeader; }
    const EventHeader* payload() const { return mPayload; }
    const EventPayload<E>* operator->() const { return mPayload; }

private:

    EventHeader* mHeader;
    EventPayload<E>* mPayload;

    static EventPayload<E>* extractPayload(EventHeader* header){

        // factory may pass nullptr if allocation failed
        if(!header){
            return nullptr;
        }

        // verify type consistency.
        AssertExit(header->type == E, "EventView", "invalid type");

        // cast the header pointer to the block type. the block always starts with the header.
        EventBlock<E>* block = reinterpret_cast<EventBlock<E>*>(header);
        return &block->payload;
    }

};

} // namespace