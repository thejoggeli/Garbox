#pragma once

#include <cstdint>

namespace Garbox {

class Crc {
public:

    static uint8_t Crc8(const uint8_t* data, uint32_t len, uint8_t initial);

};

} // namespace