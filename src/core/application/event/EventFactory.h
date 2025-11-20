#pragma once

#include <cstdint>
#include <functional>
#include "core/application/event/types/EventBlock.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventHeader.h"
#include "core/assert/Assert.h"
#include "core/util/container/DataPoolHeap.h"

namespace Garbox {

class EventFactory {
public:

    EventFactory();
    ~EventFactory();

    void init(size_t poolSizeBytes);
    void clearDataPool();

    template<typename EventPayload>
    EventWrite<EventPayload> make(ComponentDescriptor componentDescriptor){

        // shorthand for events with given payload type
        using EventBlockType = EventBlock<EventPayload>;
        using EventWriteType = EventWrite<EventPayload>;
        
        // allocate memory for the event block
        EventBlockType* block = mPool.allocate<EventBlockType>();
        if(!block){
            TriggerDebug("EventFactory", "failed to allocate event");
            return EventWriteType(nullptr);
        }

        // fill header data
        EventHeader* header = &block->header;
        header->id = getNextEventId();
        header->type = EventPayload::Type;
        header->sender = componentDescriptor;

        // the caller gets only a non-owning view with pointers to event header and payload 
        return EventWriteType(header);
    }

private:

    int32_t mCurrentEventId = 0;
    bool mInitialized = false;
    DataPoolHeap mPool;

    int32_t getNextEventId(){
        mCurrentEventId++;
        // detect overlow
        if(mCurrentEventId == 0){
            mCurrentEventId = 1; // zero is reserved for "invalid event id"
        }
        return mCurrentEventId;
    }

};

} // namespace
