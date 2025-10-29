#pragma once

#include "core/time/SoftwareTimer.h"

namespace Garbox {

class McpwmPair;

class Piezo {

public:

    enum class Volume : uint8_t {
        Low = 0,
        High = 1,
    };

    Piezo();

    void init();
    void tick();

    void setFrequency(uint32_t frequency);
    void setEnabled(bool enabled);
    void setDutyPercent(float duty);
    void setVolume(Volume volume);

private:

    void updatePwmEnable();

    McpwmPair& mPwm;
    SoftwareTimer mTestTimer;

    bool mEnabled = false;
    Volume mVolume = Volume::Low;

};

} // namespace