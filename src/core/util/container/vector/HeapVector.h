#pragma once

#include "core/util/container/storage/HeapStorage.h"
#include "core/util/container/vector/VectorBase.h"

namespace Garbox {

template <typename T>
class HeapVector : public VectorBase<T, HeapStorage<T>> {
public:

    HeapVector(std::size_t elementCount):
        // initialize members
        VectorBase<T, HeapStorage<T>>(elementCount){
        // nothing to do
    }
};

} // namespace Garbox
