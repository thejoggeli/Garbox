#pragma once

#include <array>
#include <cstdint>
#include "core/util/container/ringbuffer/HeapRingBuffer.h"
#include "core/util/container/vector/HeapVector.h"

namespace Garbox {

class DirtyDispatcher {
public:

    using HandlerFunction = void (*)(void* context);

    struct HandlerEntry {
        HandlerFunction function;
        void* context;
        bool dirty;
    };

    DirtyDispatcher(size_t capacity);

    void registerHandler(HandlerFunction function, void* context);
    void markDirty(uint32_t index);
    void markAllDirty();
    void clearAllDirty();
    void dispatch();

    uint32_t getDirtyCount() const { return mQueue.size(); }

private:
    bool mInitialized = false;
    HeapVector<HandlerEntry> mHandlers;
    HeapRingBuffer<HandlerEntry*> mQueue; 

};

} // namespace Garbox
