// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/hardware/ledc/LedcTimer.h"
#include "core/hardware/ledc/LedcChannel.h"

namespace Garbox {

class LedcInstances {
public:

    static constexpr int32_t PinStatusLed0Channel = 47; 
    static constexpr int32_t PinStatusLed1Channel = 21; 
    static constexpr int32_t PinStatusLed2Channel = 40; 
    static constexpr int32_t PinStatusLed3Channel = 39; 
    static constexpr int32_t PinBacklightChannel = 16; 
    static constexpr int32_t PinFanSpeedChannel = 13; 
    static constexpr int32_t PinPiezoChannel = 38; 

    static void Init();

    static LedcTimer& GetDimmingTimer();
    static LedcTimer& GetFanSpeedTimer();
    static LedcTimer& GetPiezoTimer();

    static LedcChannel& GetStatusLed0Channel();
    static LedcChannel& GetStatusLed1Channel();
    static LedcChannel& GetStatusLed2Channel();
    static LedcChannel& GetStatusLed3Channel();
    static LedcChannel& GetBacklightChannel();
    static LedcChannel& GetFanSpeedChannel();
    static LedcChannel& GetPiezoChannel();

};

} // namespace