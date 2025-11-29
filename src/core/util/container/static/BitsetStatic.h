#pragma once

#include "core/util/container/base/BitsetBase.h"
#include "core/util/container/storage/StaticStorage.h"

namespace Garbox {

template <std::size_t BitCount>
class BitsetStatic : public BitsetBase<StaticStorage<std::uint8_t, (BitCount + 7) / 8>>{
public:
    BitsetStatic() : BitsetBase<StaticStorage<std::uint8_t, (BitCount + 7) / 8>>(BitCount) {}
};

} // namespace Garbox
