#pragma once

#include <cstdint>

namespace Garbox {

class SpiConfig {
public:

    static constexpr uint32_t SpiDmaMaxTransferSize = ((320 * 240) * 16) / 4;

};

} // namespace Garbox
