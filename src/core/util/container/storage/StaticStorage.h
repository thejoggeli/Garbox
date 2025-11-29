#pragma once

#include <cstdint>

namespace Garbox {

template <typename T, std::size_t N>
class StaticStorage {
public:

    StaticStorage(){
        // nothing to do
    }

    uint8_t* dataBytes() {
        return mBytes;
    }

    const uint8_t* dataBytes() const {
        return mBytes;
    }

    T* dataElements() {
        return reinterpret_cast<T*>(mBytes);
    }

    const T* dataElements() const {
        return reinterpret_cast<const T*>(mBytes);
    }

    T* elementPtr(std::size_t index) {
        return reinterpret_cast<T*>(mBytes + index * sizeof(T));
    }

    const T* elementPtr(std::size_t index) const {
        return reinterpret_cast<const T*>(mBytes + index * sizeof(T));
    }

    std::size_t capacityElements() const {
        return N;
    }

    std::size_t capacityBytes() const {
        return N * sizeof(N);
    }

private:
    alignas(T) uint8_t mBytes[N * sizeof(T)];
};

} // namespace Garbox
