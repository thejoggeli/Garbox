#include "DebugLeds.h"

#include <array>
#include "assert/Assert.h"
#include "config/PinConfig.h"
#include "config/LedcPwmConfig.h"

namespace Garbox {

static constexpr uint32_t PwmFrequencyHz = 5000;
static constexpr uint32_t PwmResolutionBits = 10;

static constexpr size_t NumLeds = 4;

static std::array<Garbox::LedcPwm, NumLeds> sLeds = {
    LedcPwm(PinConfig::DebugLed0, LedcPwmConfig::DebugLed0),
    LedcPwm(PinConfig::DebugLed1, LedcPwmConfig::DebugLed1),
    LedcPwm(PinConfig::DebugLed2, LedcPwmConfig::DebugLed2),
    LedcPwm(PinConfig::DebugLed3, LedcPwmConfig::DebugLed3),
};

void DebugLeds::Init(){
    for(LedcPwm& led : sLeds){
        led.init();
        led.setDutyRaw(0);
    }
}

void DebugLeds::ToggleLed(LedId ledId, float brightness){
    size_t const index = static_cast<size_t>(ledId);
    if(index >= NumLeds){
        AssertDebug(false, "DebugLeds::ToggleLed() invalid id");
        return;
    }
    LedcPwm& led = sLeds[index];
    if(led.getDuty() == 0){
        led.setDutyNormalized(brightness);
    }
    else {
        led.setDutyRaw(0);
    }
}

void DebugLeds::SetLed(LedId ledId, bool enable, float brightness){
    size_t const index = static_cast<size_t>(ledId);
    if(index >= NumLeds){
        AssertDebug(false, "DebugLeds::SetLed() invalid id");
        return;
    }
    if(enable){
        sLeds[index].setDutyNormalized(brightness);
    }
    else {
        sLeds[index].setDutyRaw(0);
    }
}

void DebugLeds::SetAllLeds(bool enable, float brightness){
    if(enable){
        for(LedcPwm& led : sLeds){
            led.setDutyNormalized(brightness);
        }
    } 
    else {
        for(LedcPwm& led : sLeds){
            led.setDutyRaw(0);
        }
    }
}

}