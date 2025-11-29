#pragma once

#include "core/util/container/base/BitsetBase.h"
#include "core/util/container/storage/HeapStorage.h"

namespace Garbox {

class BitsetHeap : public BitsetBase<HeapStorage<std::uint8_t>> {
public:
    BitsetHeap(std::size_t bitCount) : BitsetBase<HeapStorage<std::uint8_t>>(bitCount, (bitCount + 7) / 8) {}
};

} // namespace Garbox
