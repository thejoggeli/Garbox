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
        return &mBytes[0];
    }

    const uint8_t* dataBytes() const {
        return &mBytes[0];
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
