#include "DebugLeds.h"

#include <Adafruit_NeoPixel.h>
#include <array>
#include "assert/Assert.h"
#include "global/PinConfig.h"
#include "global/ledc/LedcInstances.h"

namespace Garbox {

// Debug LEDs
static constexpr size_t NumDebugLeds = 4;
static std::array<Garbox::LedcChannel*, NumDebugLeds> sLeds = {
    &LedcInstances::GetDebugLed0Channel(),
    &LedcInstances::GetDebugLed1Channel(),
    &LedcInstances::GetDebugLed2Channel(),
    &LedcInstances::GetDebugLed3Channel()
};

// RGB LED
static constexpr uint16_t NumRgbLeds = 1;
static Adafruit_NeoPixel gPixel(NumRgbLeds, PinConfig::RgbLed, NEO_GRB + NEO_KHZ800);

// initialized flag
static bool gInitialized = false;

void DebugLeds::Init(){

    // check if already initialized
    if(gInitialized){
        AssertExit(false, "DeubgLeds already initialized");
        return;
    }

    // init RGB LED
    gPixel.begin();

    // set initialized flag
    gInitialized = true;
}

void DebugLeds::ToggleLed(Id id, float brightness){

    // check if initialized
    if(!gInitialized){
        AssertDebug(false, "DeubgLeds not initialized");
        return;
    }

    // check if valid id
    size_t const index = static_cast<size_t>(id);
    if(index >= sLeds.size()){
        AssertDebug(false, "DebugLeds::ToggleLed() invalid id");
        return;
    }

    // toggle led
    if(sLeds[index]->getDuty() == 0){
        sLeds[index]->setDutyRelative(brightness); // set debug led to brightness
    }
    else {
        sLeds[index]->setDutyRaw(0); // turn debug led off
    }
}

void DebugLeds::SetLed(Id id, bool enable, float brightness){

    // check if initialized
    if(!gInitialized){
        AssertDebug(false, "DeubgLeds not initialized");
        return;
    }

    // check if valid id
    size_t const index = static_cast<size_t>(id);
    if(index >= sLeds.size()){
        AssertDebug(false, "DebugLeds::SetLed() invalid id");
        return;
    }

    // set led
    if(enable){
        sLeds[index]->setDutyRelative(brightness); // set debug led to brightness
    }
    else {
        sLeds[index]->setDutyRaw(0); // turn debug led off
    }
}

void DebugLeds::SetAllLeds(bool enable, float brightness){

    // check if initialized
    if(!gInitialized){
        AssertDebug(false, "DeubgLeds not initialized");
        return;
    }

    // set leds
    if(enable){
        // set all debug leds to brightness
        for(LedcChannel* led : sLeds){
            led->setDutyRelative(brightness);
        }
    } 
    else {
        // turn all debug leds off
        for(LedcChannel* led : sLeds){
            led->setDutyRaw(0);
        }
    }
}

void DebugLeds::SetRgbLed(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness){

    // check if initialized
    if(!gInitialized){
        AssertDebug(false, "DeubgLeds not initialized");
        return;
    }

    // set pixel color
    gPixel.setPixelColor(0, r, g, b);
    gPixel.setBrightness(brightness);
    gPixel.show();
}

}