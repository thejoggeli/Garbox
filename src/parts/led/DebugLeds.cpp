#include "DebugLeds.h"

#include <Adafruit_NeoPixel.h>
#include <array>
#include "LedAnimationTask.h"
#include "assert/Assert.h"
#include "global/PinConfig.h"
#include "global/hardware/ledc/LedcInstances.h"
#include "util/threading/LockGuard.h"

namespace Garbox {

// Debug LEDs
static constexpr size_t NumDebugLeds = static_cast<size_t>(DebugLeds::Id::Count);
static std::array<AnimatedLedAsync, NumDebugLeds> sLeds = {
    AnimatedLedAsync(LedcInstances::GetDebugLed0Channel()),
    AnimatedLedAsync(LedcInstances::GetDebugLed1Channel()),
    AnimatedLedAsync(LedcInstances::GetDebugLed2Channel()),
    AnimatedLedAsync(LedcInstances::GetDebugLed3Channel()),
};

// RGB LED
static constexpr uint16_t NumRgbLeds = 1;
static Adafruit_NeoPixel sPixel(NumRgbLeds, PinConfig::RgbLed, NEO_GRB + NEO_KHZ800);

// LED animation task
static LedAnimationTask sAnimationTask(NumDebugLeds);

// initialized flag
static bool gInitialized = false;

// mutex for thread safety
static SemaphoreHandle_t sMutex = xSemaphoreCreateMutex();

void DebugLeds::Init(){

    Garbox::LockGuard lock(sMutex);

    AssertExit(!gInitialized, "DebugLeds", "already initialized");

    // initialize leds
    for(AnimatedLed& led : sLeds){
        led.init();
    }

    // init RGB LED
    sPixel.begin();
    sPixel.setBrightness(255);

    // setup and start animation task
    for(AnimatedLedAsync& led : sLeds){
        sAnimationTask.registerLed(&led);
    }
    sAnimationTask.start();

    // init complete
    gInitialized = true;
}

AnimatedLed& DebugLeds::GetLed(Id id){
    // check if initialized
    if(!gInitialized){
        TriggerDebug("DebugLeds", "not initialized");
        return sLeds[0];
    }

    // check if valid id
    size_t const index = static_cast<size_t>(id);
    if(index >= sLeds.size()){
        TriggerDebug("DebugLeds", "invalid id");
        return sLeds[0];
    }

    return sLeds[index];
}

Garbox::Span<AnimatedLed*> DebugLeds::GetAllLeds(){

    Garbox::LockGuard lock(sMutex);

    if(!gInitialized){
        TriggerDebug("DebugLeds", "not initialized");
        return Garbox::Span<AnimatedLed*>(nullptr, 0);
    }

    // static array of pointers to base type
    static std::array<AnimatedLed*, NumDebugLeds> sLedPtrs;

    // fill with base class pointers
    for(size_t i = 0; i < sLeds.size(); ++i){
        sLedPtrs[i] = &sLeds[i];
    }

    return Garbox::Span<AnimatedLed*>(sLedPtrs.data(), sLedPtrs.size());
}

void DebugLeds::SetLed(Id id, bool enable, float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!gInitialized){
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
        sLeds[index].setBrightness(brightness); // set debug led to brightness
    }
    else {
        sLeds[index].setBrightness(0); // turn debug led off
    }
}

void DebugLeds::ToggleLed(Id id, float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!gInitialized){
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
    const float thresh = brightness*0.5f;
    if(sLeds[index].getBrightness() < thresh){
        sLeds[index].setBrightness(brightness); // set debug led to brightness
    }
    else {
        sLeds[index].setBrightness(0); // turn debug led off
    }
}

void DebugLeds::SetAllLeds(bool enable, float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!gInitialized){
        TriggerDebug("DebugLeds", "not initialized");
        return;
    }

    // set leds
    if(enable){
        // set all debug leds to brightness
        for(AnimatedLed& led : sLeds){
            led.setBrightness(brightness);
        }
    } 
    else {
        // turn all debug leds off
        for(AnimatedLed& led : sLeds){
            led.setBrightness(0);
        }
    }
}

void DebugLeds::ToggleAllLeds(float brightness){

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if(!gInitialized){
        TriggerDebug("DebugLeds", "not initialized");
        return;
    }

    // toggle all leds
    const float thresh = brightness*0.5f;
    for(AnimatedLed& led : sLeds){
        // toggle led
        if(led.getBrightness() < thresh){
            led.setBrightness(brightness); // set debug led to brightness
        }
        else {
            led.setBrightness(0); // turn debug led off
        }
    }
}

void DebugLeds::SetRgbLed(uint8_t r, uint8_t g, uint8_t b) {

    Garbox::LockGuard lock(sMutex);

    // check if initialized
    if (!gInitialized) {
        return;
    }

    // set pixel color
    sPixel.setPixelColor(0, r, g, b);
    sPixel.show();    
}

bool DebugLeds::IsInitialized() {
    return gInitialized;
}

}
