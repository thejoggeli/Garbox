#pragma once

#include <cstdint>
#include <functional>
#include "core/event/Event.h"
#include "core/event/EventWrapper.h"
#include "core/util/container/DataPoolHeap.h"

namespace Garbox {

class EventFactory {
public:

    EventFactory();
    ~EventFactory();

    void init(size_t poolSizeBytes);
    void clearDataPool();

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
        return EventWrapper<EventData>(header);
    }

private:

    bool mInitialized = false;
    DataPoolHeap mPool;

};

} // namespace
