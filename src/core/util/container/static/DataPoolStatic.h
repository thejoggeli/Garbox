#pragma once

#include "core/util/container/base/DataPoolBase.h"
#include "core/util/container/storage/StaticStorage.h"

namespace Garbox {

template<std::size_t CapacityBytes>
class DataPoolStatic : public DataPoolBase<StaticStorage<uint8_t, CapacityBytes>> {
public:
    DataPoolStatic() : DataPoolBase<StaticStorage<uint8_t, CapacityBytes>>() {}
};

} // namespace Garbox
