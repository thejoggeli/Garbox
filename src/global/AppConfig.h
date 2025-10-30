#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    static constexpr uint32_t ClockFrequency = 80'000'000U;

    static constexpr uint32_t TargetTickRateHz = 100;
    static constexpr uint32_t TargetTickIntervalMillis = (1000) / TargetTickRateHz;
    static constexpr uint32_t TargetTickIntervalMicros = (1000 * 1000) / TargetTickRateHz;

};

} // namespace