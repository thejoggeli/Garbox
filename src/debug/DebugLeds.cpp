#include "DebugLeds.h"

#include "assert/Assert.h"
#include "config/GpioConfig.h"

namespace Garbox {

std::array<Gpio, DebugLeds::NumLeds> DebugLeds::sLeds = {
    Gpio(GpioConfig::DEBUG_LED_0),
    Gpio(GpioConfig::DEBUG_LED_1),
    Gpio(GpioConfig::DEBUG_LED_2),
    Gpio(GpioConfig::DEBUG_LED_3),
};

void DebugLeds::Init(){
    for(Gpio& gpio : sLeds){
        gpio.setMode(Gpio::Mode::Output);
    }
}

void DebugLeds::ToggleLed(LedId led){
    size_t id = static_cast<size_t>(led);
    if(id >= NumLeds){
        AssertDebug(false, "DebugLeds::ToggleLed() invalid id");
        return;
    }
    sLeds[id].toggle();
}

void DebugLeds::SetLed(LedId led, bool state){
    size_t id = static_cast<size_t>(led);
    if(id >= NumLeds){
        AssertDebug(false, "DebugLeds::SetLed() invalid id");
        return;
    }
    sLeds[id].digitalWrite(state);
}

void DebugLeds::SetAllLeds(bool state){
    for(Gpio& gpio : sLeds){
        gpio.digitalWrite(state);
    }
}

}