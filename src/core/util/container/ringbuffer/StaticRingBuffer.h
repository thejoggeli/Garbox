#pragma once

#include "core/util/container/storage/StaticStorage.h"
#include "core/util/container/ringbuffer/RingBufferBase.h"

namespace Garbox {

template <typename T, std::size_t N>
class StaticRingBuffer : public RingBufferBase<T, StaticStorage<T, N>> {
public:

    StaticRingBuffer():
        // initialize members
        RingBufferBase<T, StaticStorage<T, N>>() {
        // nothing to do
    }
};

} // namespace Garbox
