#pragma once

#include "core/util/container/base/RingBufferBase.h"
#include "core/util/container/storage/HeapStorage.h"

namespace Garbox {

template <typename T>
class RingBufferHeap : public RingBufferBase<T, HeapStorage<T>> {
public:
    RingBufferHeap(std::size_t capacityElements) : RingBufferBase<T, HeapStorage<T>>(capacityElements) {}
};

} // namespace Garbox
