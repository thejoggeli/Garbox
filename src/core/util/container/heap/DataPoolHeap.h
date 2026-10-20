#pragma once

#include "core/util/container/base/DataPoolBase.h"
#include "core/util/container/storage/HeapStorage.h"

namespace Garbox {

class DataPoolHeap : public DataPoolBase<::Garbox::HeapStorage<uint8_t>> {
public:
    DataPoolHeap(std::size_t capacityBytes) : DataPoolBase<::Garbox::HeapStorage<uint8_t>>(capacityBytes) {}
};

} // namespace Garbox
