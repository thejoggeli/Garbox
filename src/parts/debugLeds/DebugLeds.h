#pragma once

#include <cstdint>
#include <array>
#include "assert/Assert.h"
#include "global/PinConfig.h"
#include "global/ledc/LedcInstances.h"

namespace Garbox {

class DebugLeds {
public:
    enum class Id : uint8_t {
        Heartbeat = 0, 
        Custom1   = 1, 
        Custom2   = 2, 
        Assert    = 3,
    };

    static void Init();
    
    static void SetLed(Id id, bool enable, float brightness = 1.0);
    static void SetAllLeds(bool enable, float brightness = 1.0);

    static void ToggleLed(Id id, float brightness = 1.0);
    static void ToggleAllLeds(float brightness = 1.0);

    static void SetRgbLed(uint8_t r, uint8_t g, uint8_t b);

private:
    DebugLeds() = delete;
    ~DebugLeds() = delete;
};

} // namespace Garbox
