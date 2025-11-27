#pragma once

#include <cstddef>
#include <cstdint>
#include "core/util/container/storage/StaticStorage.h"
#include "BitsetBase.h"

namespace Garbox {

template <std::size_t BitCount>
class StaticBitset : public BitsetBase<StaticStorage<std::uint8_t, (BitCount + 7) / 8>>{
public:
    StaticBitset() : BitsetBase<StaticStorage<std::uint8_t, (BitCount + 7) / 8>>(BitCount) {}
};

} // namespace Garbox
