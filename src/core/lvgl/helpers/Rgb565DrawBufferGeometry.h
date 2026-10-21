#pragma once

#include <cstdint>

namespace Garbox {

struct Rgb565DrawBufferGeometry final {
    static constexpr uint64_t BytesPerPixel = 2U;

    static constexpr uint64_t strideBytes(uint32_t width){
        return static_cast<uint64_t>(width) * BytesPerPixel;
    }

    static constexpr uint64_t sizeBytes(uint32_t width, uint32_t height){
        return strideBytes(width) * static_cast<uint64_t>(height);
    }
};

} // namespace Garbox
