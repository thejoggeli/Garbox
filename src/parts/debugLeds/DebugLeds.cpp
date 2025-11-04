#include "DebugLeds.h"

#include <Adafruit_NeoPixel.h>
#include <array>
#include "assert/Assert.h"
#include "global/PinConfig.h"
#include "global/ledc/LedcInstances.h"
#include "util/threading/LockGuard.h"

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
static Adafruit_NeoPixel sPixel(NumRgbLeds, PinConfig::RgbLed, NEO_GRB + NEO_KHZ800);

// initialized flag
static bool sInitialized = false;

// mutex for thread safety
static SemaphoreHandle_t sMutex = xSemaphoreCreateMutex();

void DebugLeds::Init(){

    Garbox::LockGuard lock(sMutex);

    AssertExit(!sInitialized, "DebugLeds", "already initialized");

    // init RGB LED
    sPixel.begin();
    sPixel.setBrightness(255);

    // init complete
    sInitialized = true;
}

void DebugLeds::SetLed(Id id, bool enable, float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!sInitialized){
        TriggerDebug("DeubgLeds", "not initialized");
        return;
    }

    // check if valid id
    size_t const index = static_cast<size_t>(id);
    if(index >= sLeds.size()){
        TriggerDebug("DebugLeds", "invalid id");
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

void DebugLeds::ToggleLed(Id id, float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!sInitialized){
        TriggerDebug("DebugLeds", "not initialized");
        return;
    }

    // check if valid id
    size_t const index = static_cast<size_t>(id);
    if(index >= sLeds.size()){
        TriggerDebug("DebugLeds", "invalid id");
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

void DebugLeds::SetAllLeds(bool enable, float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!sInitialized){
        TriggerDebug("DebugLeds", "not initialized");
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

void DebugLeds::ToggleAllLeds(float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!sInitialized){
        TriggerDebug("DebugLeds", "not initialized");
        return;
    }

    // toggle all leds
    for(LedcChannel* led : sLeds){
        // toggle led
        if(led->getDuty() == 0){
            led->setDutyRelative(brightness); // set debug led to brightness
        }
        else {
            led->setDutyRaw(0); // turn debug led off
        }
    }
}

void DebugLeds::SetRgbLed(uint8_t r, uint8_t g, uint8_t b) {

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if (!sInitialized) {
        return;
    }

    // set pixel color
    sPixel.setPixelColor(0, r, g, b);
    sPixel.show();    
}

}
