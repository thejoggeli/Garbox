#pragma once

#include "core/hardware/bus/SpiDma.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class LedcChannel;

class Display {

public:

    Display();

    void init();
    void tick();

private:

    SpiDma& mSpi;
    LedcChannel& mBacklightPwm;
    SoftwareTimer mTestTimer;

    static void handleTxComplete(void* user);

};

} // namespace