#pragma once

#include "core/LedcPwm.h" 

namespace Garbox {

class LedcPwmConfig {
public:

    // Debug Leds
    static constexpr LedcPwm::Channel DebugLed0 = LedcPwm::Channel::Channel0;
    static constexpr LedcPwm::Channel DebugLed1 = LedcPwm::Channel::Channel1;
    static constexpr LedcPwm::Channel DebugLed2 = LedcPwm::Channel::Channel2;
    static constexpr LedcPwm::Channel DebugLed3 = LedcPwm::Channel::Channel3;

    // Fan
    static constexpr LedcPwm::Channel FanSpeed = LedcPwm::Channel::Channel4;

    // Display
    static constexpr LedcPwm::Channel DisplayBacklight = LedcPwm::Channel::Channel5;

    // Piezo
    static constexpr LedcPwm::Channel Piezo = LedcPwm::Channel::Channel6;

};

} // namespace