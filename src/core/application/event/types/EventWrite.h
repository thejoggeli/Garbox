#pragma once

#include "core/application/event/types/EventBlock.h"
#include "core/application/event/types/EventHeader.h"
#include "core/assert/Assert.h"

namespace Garbox {

template<typename EventPayload>
class EventWrite {
public:

    EventHeader* header;
    EventPayload* payload;

    EventWrite(EventHeader* headerPtr):
        header(headerPtr),
        payload(extractPayload(header)){
        // constructor body
    }

private:

    static EventPayload* extractPayload(EventHeader* header){

        // shorthand for block with payload type
        using EventBlockType = EventBlock<EventPayload>;

        // factory may pass nullptr if allocation failed
        if(!header){
            return nullptr;
        }

        // verify type consistency.
        AssertExit(header->type == EventPayload::Type, "EventWrite", "invalid type");

        // cast the header pointer to the block type. the block always starts with the header.
        EventBlockType* block = reinterpret_cast<EventBlockType*>(header);
        return &block->payload;
    }

};

} // namespace
