#pragma once

#include <cstdint>
#include <array>
#include "core/Gpio.h"

namespace Garbox {


class DebugLeds {
public:

    enum class LedId : uint8_t {
        Heartbeat = 0, 
        Led1 = 1, 
        Led2 = 2, 
        AssertDebug = 3,
    };

    static void Init();
    static void Start();

    static void ToggleLed(LedId led);
    static void SetLed(LedId led, bool state);
    static void SetAllLeds(bool state);

private:

    DebugLeds() = delete;
    ~DebugLeds() = delete;

    static constexpr size_t NumLeds = 4;

    static std::array<Garbox::Gpio, NumLeds> sLeds;

};

}



