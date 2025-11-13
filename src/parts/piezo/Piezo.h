#pragma once

#include "core/time/SoftwareTimer.h"

namespace Garbox {

class LedcTimer;
class LedcChannel;

class Piezo {

public:

    Piezo(LedcTimer& pwmTimer, LedcChannel& pwmChannel);

    void init();

    void setFrequency(uint32_t frequency);
    void setEnabled(bool enabled);
    void setDuty(float duty);
    
    uint32_t getFrequency();
    bool isEnabled();
    float getDuty();

    // Disallow copy and move 
    Piezo(const Piezo&) = delete;
    Piezo& operator=(const Piezo&) = delete;
    Piezo(Piezo&&) = delete;
    Piezo& operator=(Piezo&&) = delete;

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