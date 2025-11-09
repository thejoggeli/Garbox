#include "GpioInstances.h"

#include "assert/Assert.h"
#include "global/PinConfig.h"

namespace Garbox {

using Mode = Gpio::Mode;

static bool gInitialized = false;
inline static void InitGpio(Gpio& gpio, int32_t pin, Mode mode, bool invert = false, bool initialLevel = false){
    gpio.init(pin, mode, invert, initialLevel);
}

void GpioInstances::Init(){
    AssertExit(!gInitialized, "GpioInstances", "already initialized");

    // init input gpios                Pin                        Mode         Invert
    InitGpio(GetRotaryEncoderButton(), PinConfig::RotaryEncoderC, Mode::Input, true);

    // init output gpios      Pin                       Mode          Invert  Level
    InitGpio(GetDebugGpio0(), PinConfig::DebugGpio0,    Mode::Output, false,  false);
    InitGpio(GetDebugGpio1(), PinConfig::DebugGpio1,    Mode::Output, false,  false);
    InitGpio(GetFanEnable(),  PinConfig::FanEnable,     Mode::Output, false,  false);
    InitGpio(GetDisplayDc(),  PinConfig::DisplayDc,     Mode::Output, false,  false);
    InitGpio(GetDisplayCs(),  PinConfig::DisplayCs,     Mode::Output, false,  false);
    InitGpio(GetDisplayRst(), PinConfig::DisplayRst,    Mode::Output, false,  false);
    InitGpio(GetHeatEnable(), PinConfig::HeatpadEnable, Mode::Output, false,  false);

    gInitialized = true;
}

Gpio& GpioInstances::GetDebugGpio0(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetDebugGpio1(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetFanEnable(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayDc(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayCs(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetDisplayRst(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetHeatEnable(){ static Gpio instance; return instance; }
Gpio& GpioInstances::GetRotaryEncoderButton() { static Gpio instance; return instance; }

} // namespace
