#pragma once

#include "core/util/container/base/MaskedArrayBase.h"
#include "core/util/container/storage/HeapStorage.h"
#include "core/util/container/heap/ArrayHeap.h"

namespace Garbox {

template<typename T>
class MaskedArrayHeap : public MaskedArrayBase<T, HeapStorage<T>, ArrayHeap<size_t>, ArrayHeap<bool>> {
public:
    MaskedArrayHeap(std::size_t elementCount, bool defaultEnabled) : MaskedArrayBase<T, HeapStorage<T>, ArrayHeap<size_t>, ArrayHeap<bool>>(elementCount, defaultEnabled) {}
};

} // namespace Garbox
