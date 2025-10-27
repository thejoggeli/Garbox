#include "DebugLeds.h"

#include <Adafruit_NeoPixel.h>
#include <array>
#include "assert/Assert.h"
#include "config/PinConfig.h"
#include "config/LedcPwmConfig.h"

namespace Garbox {

// Debug LEDs config
static constexpr size_t NumDebugLeds = 4;

// Debug LEDs PWM config
static constexpr uint32_t PwmFrequencyHz = 5000;
static constexpr uint32_t PwmResolutionBits = 10;

// RGB LED config
static constexpr uint16_t NumRgbLeds = 1;

// RGB LED pixel instance
static Adafruit_NeoPixel gPixel(NumRgbLeds, PinConfig::RgbLed, NEO_GRB + NEO_KHZ800);

static std::array<Garbox::LedcPwm, NumDebugLeds> sLeds = {
    LedcPwm(PinConfig::DebugLed0, LedcPwmConfig::DebugLed0),
    LedcPwm(PinConfig::DebugLed1, LedcPwmConfig::DebugLed1),
    LedcPwm(PinConfig::DebugLed2, LedcPwmConfig::DebugLed2),
    LedcPwm(PinConfig::DebugLed3, LedcPwmConfig::DebugLed3),
};

void DebugLeds::Init(){

    // init debug LEDs
    for(LedcPwm& led : sLeds){
        led.init();
        led.setDutyRaw(0);
    }

    // init RGB LED
    gPixel.begin();
}

void DebugLeds::ToggleLed(LedId ledId, float brightness){
    size_t const index = static_cast<size_t>(ledId);
    if(index >= NumDebugLeds){
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
    if(index >= NumDebugLeds){
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

void DebugLeds::SetRgbLed(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness){
    gPixel.setPixelColor(0, r, g, b);
    gPixel.setBrightness(brightness);
    gPixel.show();
}

}