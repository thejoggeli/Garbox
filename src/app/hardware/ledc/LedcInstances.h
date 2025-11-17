#pragma once

#include "core/hardware/ledc/LedcTimer.h"
#include "core/hardware/ledc/LedcChannel.h"

namespace Garbox {

class LedcInstances {
public:

    static void Init();

    static LedcTimer& GetDimmingTimer();
    static LedcTimer& GetFanTimer();
    static LedcTimer& GetPiezoTimer();

    static LedcChannel& GetDebugLed0Channel();
    static LedcChannel& GetDebugLed1Channel();
    static LedcChannel& GetDebugLed2Channel();
    static LedcChannel& GetDebugLed3Channel();
    static LedcChannel& GetDisplayBacklightChannel();
    static LedcChannel& GetFanSpeedChannel();
    static LedcChannel& GetPiezoChannel();

};

} // namespace Garbox
