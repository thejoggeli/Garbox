#pragma once

namespace Garbox {

class Gpio;

struct GpioInstances {
public:

    static void Init();

    // debug pin
    static Gpio& GetDebugGpio();

    // fan pins
    static Gpio& GetFanEnable();
    static Gpio& GetFanTacho();

    // temperature sensor enable pin
    // logical high = enable
    // logical low = disable (cuts the 3v3 power on the i2c connector) 
    static Gpio& GetI2cEnablePower();

    // display pin
    static Gpio& GetDisplayDc();
    static Gpio& GetDisplayCs();
    static Gpio& GetDisplayRst();

    // heat enable pin
    static Gpio& GetHeatEnable();

    // encoder button pin
    static Gpio& GetEncoderButton();
    
    // rgb led pin
    static Gpio& GetRgbLed();

};

} // namespace
