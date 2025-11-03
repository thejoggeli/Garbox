#pragma once

#include "core/hardware/gpio/Gpio.h"

namespace Garbox {

struct GpioInstances {
public:

    static void Init();

    static Gpio& GetDebugLed0();
    static Gpio& GetDebugLed1();
    static Gpio& GetDebugLed2();
    static Gpio& GetDebugLed3();

    static Gpio& GetDebugGpio0();
    static Gpio& GetDebugGpio1();

    static Gpio& GetFanEnable();

    static Gpio& GetDisplayDc();
    static Gpio& GetDisplayCs();
    static Gpio& GetDisplayRst();

    static Gpio& GetHeatEnable();

};

} // namespace
