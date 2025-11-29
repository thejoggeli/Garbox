#pragma once

#include "core/util/container/base/ArrayBase.h"
#include "core/util/container/storage/HeapStorage.h"

namespace Garbox {

template<typename T>
class ArrayHeap : public ArrayBase<T, HeapStorage<T>> {
public:
    ArrayHeap(std::size_t n) : ArrayBase<T, HeapStorage<T>>(n) {}
};

} // namespace Garbox
