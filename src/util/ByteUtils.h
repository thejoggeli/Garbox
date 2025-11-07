#pragma once
#include <cstdint>

namespace Garbox {

class ByteUtils {
public:

    static constexpr uint8_t GetByte0(uint16_t value) noexcept { return static_cast<uint8_t>(value & 0xFF); }
    static constexpr uint8_t GetByte1(uint16_t value) noexcept { return static_cast<uint8_t>((value >> 8) & 0xFF); }

    static constexpr uint8_t GetByte0(uint32_t value) noexcept { return static_cast<uint8_t>(value & 0xFF); }
    static constexpr uint8_t GetByte1(uint32_t value) noexcept { return static_cast<uint8_t>((value >> 8) & 0xFF); }
    static constexpr uint8_t GetByte2(uint32_t value) noexcept { return static_cast<uint8_t>((value >> 16) & 0xFF); }
    static constexpr uint8_t GetByte3(uint32_t value) noexcept { return static_cast<uint8_t>((value >> 24) & 0xFF); }
    
    static constexpr uint16_t GetWord0(uint32_t value) noexcept { return static_cast<uint16_t>(value & 0xFFFF); }
    static constexpr uint16_t GetWord1(uint32_t value) noexcept { return static_cast<uint16_t>((value >> 16) & 0xFFFF); }

    static constexpr uint16_t MakeUint16(uint8_t low, uint8_t high) noexcept {
        return static_cast<uint16_t>((static_cast<uint16_t>(high) << 8) | low);
    }

    static constexpr uint32_t MakeUint32(uint16_t low, uint16_t high) noexcept {
        return (static_cast<uint32_t>(high) << 16) | low;
    }

    static constexpr uint32_t MakeUint32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) noexcept {
        return (static_cast<uint32_t>(b3) << 24) |
               (static_cast<uint32_t>(b2) << 16) |
               (static_cast<uint32_t>(b1) << 8)  |
                static_cast<uint32_t>(b0);
    }
};

} // namespace Garbox
