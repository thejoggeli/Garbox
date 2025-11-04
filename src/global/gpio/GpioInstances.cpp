#include "GpioInstances.h"

#include "assert/Assert.h"
#include "global/PinConfig.h"

namespace Garbox {

static bool sInitialized = false;

void GpioInstances::Init(){
    AssertExit(!sInitialized, "SpiInstances", "already initialized");

    constexpr bool InitialOff = false;

    GetDebugGpio0().setup(PinConfig::DebugGpio0,    Gpio::Mode::Output, InitialOff);
    GetDebugGpio1().setup(PinConfig::DebugGpio1,    Gpio::Mode::Output, InitialOff);
    GetFanEnable ().setup(PinConfig::FanEnable,     Gpio::Mode::Output, InitialOff);
    GetDisplayDc ().setup(PinConfig::DisplayDc,     Gpio::Mode::Output, InitialOff);
    GetDisplayCs ().setup(PinConfig::DisplayCs,     Gpio::Mode::Output, InitialOff);
    GetDisplayRst().setup(PinConfig::DisplayRst,    Gpio::Mode::Output, InitialOff);
    GetHeatEnable().setup(PinConfig::HeatpadEnable, Gpio::Mode::Output, InitialOff);

    sInitialized = true;
}

Gpio& GpioInstances::GetDebugGpio0() { static Gpio instance; return instance; }
Gpio& GpioInstances::GetDebugGpio1() { static Gpio instance; return instance; }
Gpio& GpioInstances::GetFanEnable()  { static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayDc () { static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayCs () { static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayRst() { static Gpio instance; return instance; }
Gpio& GpioInstances::GetHeatEnable() { static Gpio instance; return instance; }

} // namespace
