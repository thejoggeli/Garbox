#pragma once

#include "core/util/container/storage/HeapStorage.h"
#include "core/util/container/datapool/DataPoolBase.h"

namespace Garbox {

class HeapDataPool : public DataPoolBase<HeapStorage<uint8_t>> {
public:

    HeapDataPool(std::size_t capacityBytes):
        // initialize members
        DataPoolBase<HeapStorage<uint8_t>>(capacityBytes){
        // nothing to do
    }
};

} // namespace Garbox
