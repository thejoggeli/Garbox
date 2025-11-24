#pragma once

#include "core/util/container/storage/HeapStorage.h"
#include "core/util/container/ringbuffer/RingBufferBase.h"

namespace Garbox {

template <typename T>
class HeapRingBuffer : public RingBufferBase<T, HeapStorage<T>> {
public:

    HeapRingBuffer(std::size_t capacityElements):
        // initialize members
        RingBufferBase<T, HeapStorage<T>>(capacityElements){
        // nothing to do
    }
};

} // namespace Garbox
