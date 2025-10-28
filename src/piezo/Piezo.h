#pragma once

#include "core/LedcPwm.h"
#include "core/SoftwareTimer.h"

namespace Garbox {

class Piezo {

public:

    Piezo();

    void init();
    void tick();

    void setFrequency(uint32_t frequency);
    void setEnabled(bool enabled);
    void setDuty(float duty);

private:

    LedcPwm mPwm;
    SoftwareTimer mTestTimer;

    bool mEnabled = false;
    float mDuty = 0.5f;
    uint32_t mFrequency = 1000;
    uint8_t mResolutionBits = 10;

};

} // namespace