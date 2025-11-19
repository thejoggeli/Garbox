#include "GpioInstances.h"

#include "app/config/PinConfig.h"
#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"

namespace Garbox {

constexpr bool InitialLow = false;
constexpr bool InitialHigh = true;

constexpr bool NonInverted = false;
constexpr bool Inverted = true;

using Config = Gpio::Config;
using Mode = Gpio::Mode;
using Pull = Gpio::Pull;
using Interrupt = Gpio::Interrupt;

template<int32_t Pin>
Gpio& GetGpioInstance(){
    static Gpio instance(Pin);
    return instance;
}

static void InitGpio(Gpio& gpio, const Config& config, bool initialLevel = InitialLow){
    if(!gpio.isInitialized()){
        gpio.init(config);
    }
}

static bool gInitialized = false;

// void GpioInstances::Init(){

void GpioInstances::Init(){
    AssertExit(!gInitialized, "GpioInstances", "already initialized");

    // debug gpios
    InitGpio(GetDebugGpio(), { Mode::Disable, Pull::Disable, Interrupt::Disable, NonInverted }, InitialLow);

    // fan
    InitGpio(GetFanEnable(), { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }, InitialLow);
    InitGpio(GetFanTacho(),  { Mode::Input,  Pull::Disable, Interrupt::Disable, NonInverted });

    // i2c sensor
    InitGpio(GetI2cEnablePower(), { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }, InitialHigh);

    // display
    InitGpio(GetDisplayDc(),  { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }, InitialLow);
    InitGpio(GetDisplayCs(),  { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }, InitialHigh);
    InitGpio(GetDisplayRst(), { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }, InitialLow);

    // heatpad
    InitGpio(GetHeatEnable(), { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }, InitialLow);
    
    // rotary encoder
    InitGpio(GetEncoderButton(), { Mode::Input, Pull::Disable, Interrupt::Disable, Inverted });
    
    // rgb led
    InitGpio(GetRgbLed(), { Mode::Output, Pull::Disable, Interrupt::Disable, NonInverted }, InitialLow);

    // initialization complete
    gInitialized = true;
}

// debug gpios
Gpio& GpioInstances::GetDebugGpio(){ return GetGpioInstance<PinConfig::DebugGpio>(); }

// fan
Gpio& GpioInstances::GetFanEnable(){ return GetGpioInstance<PinConfig::FanEnable>(); }
Gpio& GpioInstances::GetFanTacho(){ return GetGpioInstance<PinConfig::FanTacho>(); }

// temperature sensor
Gpio& GpioInstances::GetI2cEnablePower(){ return GetGpioInstance<PinConfig::I2cEnable>(); }

// display
Gpio& GpioInstances::GetDisplayDc(){ return GetGpioInstance<PinConfig::DisplayDc>(); }
Gpio& GpioInstances::GetDisplayCs(){ return GetGpioInstance<PinConfig::DisplayCs>(); }
Gpio& GpioInstances::GetDisplayRst(){ return GetGpioInstance<PinConfig::DisplayRst>(); }

// heatpad
Gpio& GpioInstances::GetHeatEnable(){ return GetGpioInstance<PinConfig::HeatpadEnable>(); }

// rotary encoder
Gpio& GpioInstances::GetEncoderButton(){ return GetGpioInstance<PinConfig::RotaryEncoderC>(); }

// rgb led
Gpio& GpioInstances::GetRgbLed(){ return GetGpioInstance<PinConfig::RgbLed>(); }

} // namespace
