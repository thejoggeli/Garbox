#pragma once

#include <cstdint>

namespace Garbox {

class DebugLeds {
public:

    enum class LedId : uint8_t {
        Led0 = 0, 
        Led1 = 1, 
        Led2 = 2, 
        Led3 = 3,
    };

    static void Init();

    static void ToggleLed(LedId ledId, float brightness = 1.0);
    static void SetLed(LedId ledId, bool enable, float brightness = 1.0);
    static void SetAllLeds(bool enable, float brightness = 1.0);

    static void SetRgbLed(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness = 255);

private:

    DebugLeds() = delete;
    ~DebugLeds() = delete;

};

}



