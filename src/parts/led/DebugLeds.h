#pragma once

#include <cstdint>
#include <array>
#include "AnimatedLedAsync.h"
#include "assert/Assert.h"
#include "global/PinConfig.h"
#include "global/hardware/ledc/LedcInstances.h"
#include "util/container/Span.h"

namespace Garbox {

class DebugLeds {
public:
    enum class Id : uint8_t {
        Heartbeat = 0, 
        Custom1, 
        Custom2, 
        Assert,
        Count
    };

    static void Init();

    static AnimatedLed& GetLed(Id id);    
    static Span<AnimatedLed*> GetAllLeds();
    
    static void SetLed(Id id, bool enable, float brightness = 1.0);
    static void SetAllLeds(bool enable, float brightness = 1.0);

    static void ToggleLed(Id id, float brightness = 1.0);
    static void ToggleAllLeds(float brightness = 1.0);

    static void SetRgbLed(uint8_t r, uint8_t g, uint8_t b);

    static bool IsInitialized();

private:
    DebugLeds() = delete;
    ~DebugLeds() = delete;
};

} // namespace Garbox
