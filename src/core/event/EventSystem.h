#pragma once

#include <cstdint>
#include <functional>
#include "core/event/EventWrapper.h"
#include "util/container/DataPoolHeap.h"
#include "util/container/RingBufferHeap.h"

namespace Garbox {

class EventSystem {
public:

    using SendHandler = std::function<void(EventWrapper event)>;

    EventSystem(size_t poolSizeBytes, size_t queueSizeItems): 
        // init members
        mPool(poolSizeBytes),
        mQueue(queueSizeItems){
        // nothing to do
    }

    void init(){
        mPool.init();
        mQueue.init();
    }

    template<typename Event>
    void send(const Event& data){
        Event* event = mPool.allocate<Event>(data);
        EventWrapper wrapper(Event::Type, dataPtr);
        if(mSendHandler){
            mSendHandler(event);
        }
    }

    template<typename Event>
    void queue(const Event& data){
        Event* dataPtr = mPool.allocate<Event>(data);
        EventWrapper event(Event::Type, dataPtr);
        mQueue.push(event);
    }

    void clearPool(){
        mPool.clear();
    }

private:

    DataPoolHeap mPool;
    RingBufferHeap<EventWrapper> mQueue;

    SendHandler mSendHandler = nullptr;
};

} // namespace
