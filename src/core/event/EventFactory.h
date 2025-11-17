#pragma once

#include <cstdint>
#include <functional>
#include "core/event/Event.h"
#include "core/event/EventWrapper.h"
#include "util/container/DataPoolHeap.h"

namespace Garbox {

class EventFactory {
public:

    EventFactory(){ 
        // nothing to do
    }

    ~EventFactory(){ 
        AssertExit(!mInitialized, "EventFactory", "heap deconstructor");
    }

    void init(size_t poolSizeBytes){
        AssertExit(!mInitialized, "EventFactory", "already initialized");
        mPool.init(poolSizeBytes);
        mInitialized = true;
    }

    template<typename EventData>
    EventWrapper<EventData> make(){
        // allocate the event header
        Event* header = mPool.allocate<Event>();
        header->type = EventData::Type;
        header->sender = ControllerId::Null;

        // allocate data only if EventData is non-empty
        EventData* data = nullptr;

        if constexpr(sizeof(EventData) > 0){
            data = mPool.allocate<EventData>();
        }

        header->data = data;

        return EventWrapper<EventData>(header, data);
    }

private:

    bool mInitialized = false;
    DataPoolHeap mPool;

};

} // namespace
