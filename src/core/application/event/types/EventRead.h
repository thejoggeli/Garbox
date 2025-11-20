#pragma once

#include "core/application/event/types/EventBlock.h"
#include "core/application/event/types/EventHeader.h"
#include "core/assert/Assert.h"

namespace Garbox {

template<typename EventPayload>
class EventRead {
public:

    const EventHeader* header;
    const EventPayload* payload;

    EventRead(const EventHeader* headerPtr):
        header(headerPtr),
        payload(extractPayload(header)){
        // constructor body
    }

private:

    static const EventPayload* extractPayload(const EventHeader* header){

        // shorthand for block with payload type
        using EventBlockType = EventBlock<EventPayload>;

        // factory may pass nullptr if allocation failed
        if(!header){
            return nullptr;
        }

        // verify type consistency.
        AssertExit(header->type == EventPayload::Type, "EventRead", "invalid type");

        // cast the header pointer to the block type. the block always starts with the header.
        const EventBlockType* block = reinterpret_cast<const EventBlockType*>(header);
        return &block->payload;
    }

};

} // namespace
