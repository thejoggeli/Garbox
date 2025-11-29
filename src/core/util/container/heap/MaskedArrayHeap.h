#pragma once

#include "core/util/container/base/MaskedArrayBase.h"
#include "core/util/container/storage/HeapStorage.h"

namespace Garbox {

template<typename T>
class MaskedArrayHeap : public MaskedArrayBase<T, HeapStorage<T>> {
public:
    MaskedArrayHeap(std::size_t elementCount, bool defaultEnabled) : MaskedArrayHeap<T, HeapStorage<T>>(elementCount) {}
};

} // namespace Garbox
