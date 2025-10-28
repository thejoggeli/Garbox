#pragma once

#include <cstdint>

namespace Garbox {

class AppConfig {
public:

    static constexpr uint32_t targetTickRateHz = 100;
    static constexpr uint32_t targetTickIntervalMillis = (1000) / targetTickRateHz;
    static constexpr uint32_t targetTickIntervalMicros = (1000 * 1000) / targetTickRateHz;

};

} // namespace