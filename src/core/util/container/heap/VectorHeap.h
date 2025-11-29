#pragma once

#include "core/util/container/storage/HeapStorage.h"
#include "core/util/container/base/VectorBase.h"

namespace Garbox {

template <typename T>
class VectorHeap : public VectorBase<T, HeapStorage<T>> {
public:

    VectorHeap(std::size_t elementCount):
        // initialize members
        VectorBase<T, HeapStorage<T>>(elementCount){
        // nothing to do
    }
};

} // namespace Garbox
