#pragma once

#include <cstdint>

namespace Garbox {

class DebugLeds {
public:

    enum class Id : uint8_t {
        Heartbeat = 0, 
        Custom1 = 1, 
        Custom2 = 2, 
        Assert = 3,
    };

    static void Init();

    static void ToggleLed(Id id, float brightness = 1.0);
    static void SetLed(Id id, bool enable, float brightness = 1.0);
    static void SetAllLeds(bool enable, float brightness = 1.0);

    static void SetRgbLed(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness = 255);

private:

    DebugLeds() = delete;
    ~DebugLeds() = delete;

};

}



