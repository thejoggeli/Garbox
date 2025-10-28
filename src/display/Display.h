#pragma once

#include "core/LedcPwm.h"
#include "core/SoftwareTimer.h"

namespace Garbox {

class Display {

public:

    Display();

    void init();
    void tick();

private:

    LedcPwm mBacklightPwm;

    SoftwareTimer mTestTimer;

};

} // namespace