#pragma once

namespace Garbox {

class Gpio;

struct GpioInstances {
public:

    static void Init();

    static Gpio& GetDebugGpio0();
    static Gpio& GetDebugGpio1();

    static Gpio& GetFanEnable();
    static Gpio& GetFanTacho();

    static Gpio& GetDisplayDc();
    static Gpio& GetDisplayCs();
    static Gpio& GetDisplayRst();

    static Gpio& GetHeatEnable();

    static Gpio& GetEncoderButton();

};

} // namespace
