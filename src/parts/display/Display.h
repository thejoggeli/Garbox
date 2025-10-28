#pragma once

#include "core/time/SoftwareTimer.h"

namespace Garbox {

class LedcChannel;

class Display {

public:

    Display();

    void init();
    void tick();

private:

    LedcChannel& mBacklightPwm;

    SoftwareTimer mTestTimer;

};

} // namespace