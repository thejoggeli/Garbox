#pragma once

#include "core/util/container/storage/StaticStorage.h"
#include "core/util/container/datapool/DataPoolBase.h"

namespace Garbox {

template<std::size_t CapacityBytes>
class StaticDataPool : public DataPoolBase<StaticStorage<uint8_t, CapacityBytes>> {
public:

    StaticDataPool():
        // initialize members
        DataPoolBase<StaticStorage<uint8_t, CapacityBytes>>() {
        // nothing to do
    }
};

} // namespace Garbox
