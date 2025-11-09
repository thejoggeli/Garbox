#pragma once

#include "core/hardware/gpio/Gpio.h"

namespace Garbox {

struct GpioInstances {
public:

    static void Init();

    static Gpio& GetDebugGpio0();
    static Gpio& GetDebugGpio1();

    static Gpio& GetFanEnable();

    static Gpio& GetDisplayDc();
    static Gpio& GetDisplayCs();
    static Gpio& GetDisplayRst();

    static Gpio& GetHeatEnable();

    static Gpio& GetRotaryEncoderButton();

};

} // namespace
