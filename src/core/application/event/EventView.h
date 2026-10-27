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
 * - operator-> accesses the payload
 * - header() returns the header pointer
 * - payload() returns the payload pointer
 * - invalid views have a null header and a discard payload, so fatal handlers
 *   that return during host tests cannot turn allocation failure into a null
 *   dereference
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

    bool valid() const { return mHeader != nullptr; }
    explicit operator bool() const { return valid(); }

    // Event metadata can be configured only before submission. The raw header
    // is exposed as const so callers cannot bypass the lifetime guard.
    void setSendToInactiveComponents(bool enabled) {
        if(EventHeader* header = mutableHeader()){
            header->sendToInactiveComponents = enabled;
        }
    }
    void setBypassQueue(bool enabled) {
        if(EventHeader* header = mutableHeader()){
            header->bypassQueue = enabled;
        }
    }

    // non-const payload access is valid only while constructing the event.
    EventPayload<E>* payload() { return mutablePayload(); }
    EventPayload<E>* operator->() { return mutablePayload(); }

    // const access
    const EventHeader* header() const { return mHeader; }
    const EventPayload<E>* payload() const { return mPayload; }
    const EventPayload<E>* operator->() const { return mPayload; }

private:

    EventHeader* mHeader;
    EventPayload<E>* mPayload;

    static EventPayload<E>* invalidPayload(){
        static EventPayload<E> payload {};
        return &payload;
    }

    EventHeader* mutableHeader(){
        if(!mHeader){
            return nullptr;
        }
        if(mHeader->submitted){
            TriggerExit("EventView", "cannot modify submitted event header");
            return nullptr;
        }
        return mHeader;
    }

    EventPayload<E>* mutablePayload(){
        if(!mHeader){
            return invalidPayload();
        }
        if(mHeader->submitted){
            TriggerExit("EventView", "cannot modify submitted event payload");
            return invalidPayload();
        }
        return mPayload;
    }

    static EventPayload<E>* extractPayload(EventHeader* header){

        // Allocation failure is fatal. Host test handlers may nevertheless
        // return, so provide writable discard storage for normal payload setup
        // while preserving a null header that cannot be submitted.
        if(!header){
            return invalidPayload();
        }

        // verify type consistency.
        AssertExit(header->type == E, "EventView", "invalid type");

        // cast the header pointer to the block type. the block always starts with the header.
        EventBlock<E>* block = reinterpret_cast<EventBlock<E>*>(header);
        return &block->payload;
    }

};

} // namespace
