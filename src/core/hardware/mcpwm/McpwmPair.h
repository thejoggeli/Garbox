#pragma once

#include <cstdint>
#include "driver/mcpwm.h"
#include "esp_err.h"

namespace Garbox {

class McpwmPair {
public:
    enum class Unit : uint8_t {
        Unit0 = 0,
        Unit1 = 1,
    };

    enum class Timer : uint8_t {
        Timer0 = 0,
        Timer1 = 1,
        Timer2 = 2,
    };

    McpwmPair();
    ~McpwmPair();

    bool setup(Unit unit, Timer timer, int gpioA, int gpioB, uint32_t frequencyHz, float dutyPercent);
    bool setDutyPercent(float dutyPercent);
    bool setFrequency(uint32_t frequencyHz);

    bool setEnabled(bool enable);
    bool setEnabledA(bool enable);
    bool setEnabledB(bool enable);

private:
    Unit mUnit;
    Timer mTimer;
    int mGpioA;
    int mGpioB;
    uint32_t mFrequency;
    float mDuty;
    bool mInitialized;
    bool mEnabledA;
    bool mEnabledB;
};

} // namespace Garbox
