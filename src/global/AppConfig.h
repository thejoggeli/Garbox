#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    // Timing
    static constexpr uint32_t TargetTickRateHz = 100;
    static constexpr uint32_t TargetTickIntervalMillis = (1000) / TargetTickRateHz;
    static constexpr uint32_t TargetTickIntervalMicros = (1000 * 1000) / TargetTickRateHz;

    // FreeRTOS task priorities
    static constexpr uint32_t SpiDmaTaskPriority = 5; 

    // SPI
    static constexpr uint32_t SpiDmaMaxTransferSize = ((320 * 240) * 16) / 4;

};

} // namespace