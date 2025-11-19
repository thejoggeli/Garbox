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

    template<typename EventDataType>
    EventWrapper<EventDataType> make(ComponentDescriptor componentDescriptor){
        // allocate the event header
        Event* event = mPool.allocate<Event>();
        if(!event){
            TriggerDebug("EventWrapper", "failed to allocate event");
            return EventWrapper<EventDataType>{nullptr};
        }

        event->id = getNextEventId();
        event->type = EventDataType::Type;
        event->sender = componentDescriptor;

        // allocate data only if EventData is non-empty
        EventDataType* data = nullptr;

        if constexpr(sizeof(EventDataType) > 0){
            data = mPool.allocate<EventDataType>();
            if(!data){
                TriggerDebug("EventWrapper", "failed to allocate event data");
                return EventWrapper<EventDataType>{nullptr};
            }
        }

        event->data = data;
        return EventWrapper<EventDataType>(event);
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
