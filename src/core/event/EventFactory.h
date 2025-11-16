#pragma once

#include <cstdint>
#include <functional>
#include "core/event/Event.h"
#include "core/event/EventWrapper.h"
#include "util/container/DataPoolHeap.h"

namespace Garbox {

class EventFactory {
public:

    EventFactory(size_t poolSizeBytes): 
        // init members
        mPool(poolSizeBytes){
        // nothing to do
    }

    void init(){
        mPool.init();
    }

    template<typename EventData>
    EventWrapper<EventData> make(uint8_t sender = 0, uint8_t meta = 0){
        // allocate the event header
        Event* header = mPool.allocate<Event>();
        header->type = EventData::Type;
        header->sender = sender;
        header->meta = meta;

        // allocate data only if EventData is non-empty
        EventData* data = nullptr;

        if constexpr(sizeof(EventData) > 0){
            data = mPool.allocate<EventData>();
        }

        header->data = data;

        return EventWrapper<EventData>(header, data);
    }

private:

    DataPoolHeap mPool;

};

} // namespace
