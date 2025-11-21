#include "GpioInstances.h"

#include "core/assert/Assert.h"

namespace Garbox {

constexpr bool InitialLow = false;
constexpr bool InitialHigh = true;

constexpr bool NonInverted = false;
constexpr bool Inverted = true;

using Config = Gpio::Config;
using Mode = Gpio::Mode;
using Pull = Gpio::Pull;
using Interrupt = Gpio::Interrupt;

static bool sInitialized = false;

void GpioInstances::Init(){
    AssertExit(!sInitialized, "GpioInstances", "already initialized");

    GetDebugGpio().init(
        PinDebugGpio, InitialLow,
        { Mode::Disable, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetFanEnable().init(
        PinFanEnable, InitialLow,
        { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetFanTacho().init(
        PinFanTacho, InitialLow,
        { Mode::Input, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetI2cEnable().init(
        PinI2cEnable, InitialHigh,
        { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetDisplayDc().init(
        PinDisplayDc, InitialLow,
        { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetDisplayCs().init(
        PinDisplayCs, InitialHigh,
        { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetDisplayRst().init(
        PinDisplayRst, InitialLow,
        { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetHeatpadEnable().init(
        PinHeatpadEnable, InitialLow,
        { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }
    );
    GetEncoderButton().init(
        PinEncoderButton, InitialLow,
        { Mode::Input, Pull::Disable, Interrupt::Disable, Inverted }
    );
    GetRgbLed().init(
        PinRgbLed, InitialLow,
        { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }
    );

    sInitialized = true;
}

Gpio& GpioInstances::GetDebugGpio(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetFanEnable(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetFanTacho(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetI2cEnable(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayDc(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayCs(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayRst(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetHeatpadEnable(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetEncoderButton(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetRgbLed(){ static Gpio instance; return instance; }

} // namespace