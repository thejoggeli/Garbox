#pragma once
#include <cstddef>
#include <cstdint>
#include "core/util/container/storage/HeapStorage.h"
#include "BitsetBase.h"

namespace Garbox {

class HeapBitset : public BitsetBase<HeapStorage<std::uint8_t>> {
public:
    HeapBitset(std::size_t bitCount) : BitsetBase<HeapStorage<std::uint8_t>>(bitCount, (bitCount + 7) / 8) {}
};

} // namespace Garbox
