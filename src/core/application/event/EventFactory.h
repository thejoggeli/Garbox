#pragma once

#include <cstdint>
#include <functional>
#include "core/application/event/EventView.h"
#include "core/application/event/EventBlock.h"
#include "core/application/event/EventHeader.h"
#include "core/assert/Assert.h"
#include "core/util/container/DataPoolHeap.h"
#include "shared/types/EventType.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

class EventFactory {
public:

    EventFactory(size_t poolSizeBytes);
    ~EventFactory();

    void clearDataPool();

    template<EventType E>
    EventView<E> make(ComponentDescriptor componentDescriptor){
        
        // allocate memory for the event block
        void* memory = mPool.allocate<EventBlock<E>>();
        if(!memory){
            TriggerDebug("EventFactory", "failed to allocate event");
            return EventView<E>((EventHeader*)nullptr);
        }

        // use placement new with value-initialize (fills memory with either zeros or default values) 
        EventBlock<E>* block = new (memory) EventBlock<E>();

        // fill header data
        EventHeader* header = &block->header;
        header->id = getNextEventId();
        header->type = E;
        header->sender = componentDescriptor;

        // the caller gets only a non-owning view with pointers to event header and payload 
        return EventView<E>(header);
    }

private:

    int32_t mCurrentEventId = 0;
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
