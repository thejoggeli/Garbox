#pragma once

#include <array>
#include <cstdint>
#include "core/util/container/heap/RingBufferHeap.h"
#include "core/util/container/heap/VectorHeap.h"

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
    VectorHeap<HandlerEntry> mHandlers;
    RingBufferHeap<HandlerEntry*> mQueue; 

};

} // namespace Garbox
