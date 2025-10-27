#pragma once

#include "debug/DebugLeds.h" 

namespace Garbox {

class DebugLedsConfig {
public:

    static constexpr DebugLeds::LedId Heartbeat = DebugLeds::LedId::Led0;
    static constexpr DebugLeds::LedId Custom1 = DebugLeds::LedId::Led1;
    static constexpr DebugLeds::LedId Custom2 = DebugLeds::LedId::Led2;
    static constexpr DebugLeds::LedId Assert = DebugLeds::LedId::Led3;

};

} // namespace