#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/hardware/gpio/Gpio.h"

namespace Garbox {

struct GpioInstances {
public:

    static constexpr int32_t PinDebugGpio = 41; 
    static constexpr int32_t PinFanEnable = 4; 
    static constexpr int32_t PinFanTacho = 5; 
    static constexpr int32_t PinI2cEnable = 15; 
    static constexpr int32_t PinDisplayDc = 18; 
    static constexpr int32_t PinDisplayCs = 17; 
    static constexpr int32_t PinDisplayRst = 10; 
    static constexpr int32_t PinHeatpadEnable = 14; 
    static constexpr int32_t PinEncoderButton = 1; 
    static constexpr int32_t PinRgbLed = 48; 

    static void Init();

    static Gpio& GetDebugGpio();
    static Gpio& GetFanEnable();
    static Gpio& GetFanTacho();
    static Gpio& GetI2cEnable();
    static Gpio& GetDisplayDc();
    static Gpio& GetDisplayCs();
    static Gpio& GetDisplayRst();
    static Gpio& GetHeatpadEnable();
    static Gpio& GetEncoderButton();
    static Gpio& GetRgbLed();
};

} // namespace