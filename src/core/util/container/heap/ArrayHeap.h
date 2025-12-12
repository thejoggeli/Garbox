#pragma once

#include "core/util/container/base/ArrayBase.h"
#include "core/util/container/storage/HeapStorage.h"

namespace Garbox {

template<typename T>
class ArrayHeap : public ArrayBase<T, HeapStorage<T>> {
public:

    ArrayHeap(std::size_t n) : ArrayBase<T, HeapStorage<T>>(n) {}

    template<typename... Args>
    ArrayHeap(std::size_t elementCount, Args&&... args) : ArrayBase<T, HeapStorage<T>>(
        ArrayBase<T, HeapStorage<T>>::with_count,
        elementCount, 
        std::forward<Args>(args)...) {}

};

} // namespace Garbox
