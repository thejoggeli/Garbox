#pragma once

#include "core/time/SoftwareTimer.h"

namespace Garbox {

class LedcTimer;
class LedcChannel;

class Piezo {

public:

    Piezo();

    void init();

    void setFrequency(uint32_t frequency);
    void setEnabled(bool enabled);
    void setDuty(float duty);
    
    uint32_t getFrequency();
    bool isEnabled();
    float getDuty();

private:

    LedcTimer& mPwmTimer;
    LedcChannel& mPwmChannel;
    SoftwareTimer mTestTimer;

    bool mEnabled = false;
    float mDuty = 0.5f;
    uint32_t mFrequency = 1000;
    uint8_t mResolutionBits = 10;

};

} // namespace