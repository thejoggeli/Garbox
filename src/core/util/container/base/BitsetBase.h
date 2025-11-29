#pragma once

#include <cstdint>
#include "core/assert/Assert.h"

namespace Garbox {

template <typename Storage>
class BitsetBase : private Storage {
public:

    BitsetBase(std::size_t bitCount) : Storage(), mBitCount(bitCount) {
        const bool fits = requiredBytes(bitCount) <= Storage::capacityBytes();
        AssertExit(fits, "BitsetBase", "capacity too small for bit count");
    }

    BitsetBase(std::size_t bitCount, std::size_t elementCount) : Storage(elementCount), mBitCount(bitCount) {
        const bool fits = requiredBytes(bitCount) <= Storage::capacityBytes();
        AssertExit(fits, "BitsetBase", "capacity too small for bit count");
    }

    std::size_t size() const {
        return mBitCount;
    }

    void clearAll() {
        std::memset(Storage::dataBytes(), 0, requiredBytes(mBitCount));
    }

    void setAll() {
        std::memset(Storage::dataBytes(), 0xFF, requiredBytes(mBitCount));
        trimExtraBits();
    }

    void set(std::size_t bit) {
        AssertExit(bit < mBitCount, "Bitset", "set out-of-range");
        data()[byteIndex(bit)] |= bitMask(bit);
    }

    void clear(std::size_t bit) {
        AssertExit(bit < mBitCount, "Bitset", "clear out-of-range");
        data()[byteIndex(bit)] &= ~bitMask(bit);
    }

    void toggle(std::size_t bit) {
        AssertExit(bit < mBitCount, "Bitset", "toggle out-of-range");
        data()[byteIndex(bit)] ^= bitMask(bit);
    }

    void setValue(std::size_t bit, bool value) {
        AssertExit(bit < mBitCount, "Bitset", "setValue out-of-range");
        std::uint8_t& byte = data()[byteIndex(bit)];
        std::uint8_t mask = bitMask(bit);
        if(value) byte |= mask;
        else      byte &= ~mask;
    }

    bool getValue(std::size_t bit) const {
        AssertExit(bit < mBitCount, "Bitset", "getValue out-of-range");
        return (data()[byteIndex(bit)] & bitMask(bit)) != 0;
    }

    bool isSet(std::size_t bit) const {
        return getValue(bit);
    }

    bool isCleared(std::size_t bit) const {
        return !getValue(bit);
    }

private:
    static constexpr std::size_t requiredBytes(std::size_t bitCount) {
        return (bitCount + 7) / 8;
    }

    static constexpr std::size_t byteIndex(std::size_t bit) {
        return bit >> 3;
    }
    static constexpr std::uint8_t bitMask(std::size_t bit) {
        return static_cast<std::uint8_t>(1u << (bit & 7));
    }

    std::uint8_t* data() {
        return reinterpret_cast<std::uint8_t*>(Storage::dataBytes());
    }
    const std::uint8_t* data() const {
        return reinterpret_cast<const std::uint8_t*>(Storage::dataBytes());
    }

    // Zero out extra bits in the last byte (if size not multiple of 8)
    void trimExtraBits() {
        std::size_t extra = (mBitCount & 7);
        if(extra == 0) return;
        std::uint8_t mask = static_cast<std::uint8_t>((1u << extra) - 1u);
        data()[requiredBytes(mBitCount) - 1] &= mask;
    }

private:
    std::size_t mBitCount;
};

} // namespace Garbox
