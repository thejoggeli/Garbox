#include <Arduino.h>

#include "assert/AssertHandler.h"
#include "control/MainControl.h"
#include "core/time/Time.h"
#include "global/ledc/LedcInstances.h"
#include "parts/debugLeds/DebugLeds.h"

using namespace Garbox;

MainControl gMainControl;

void setup() {
    Serial.begin(115200); 
    
    // assert debug handler
    AssertHandler::SetDebugHandler([](const char* message){
        Serial.println(message);
        gMainControl.onAssertDebug(message);
    });

    // assert exit handler
    AssertHandler::SetExitHandler([](const char* message){
        gMainControl.onAssertExit(message);
        while(true){
            Serial.println(message);
            delay(1000);
        }
    });

    // init ledc
    LedcInstances::Init();

    // init debug leds
    DebugLeds::Init();

    // fade debug leds in
    for(int32_t i = i; i <= 25; i++){
        float brightness = static_cast<float>(i) * (1.0f/25.0f);
        DebugLeds::SetAllLeds(true, brightness);
        delay(10);
    }
    delay(250);

    // fade debug leds out
    for(int32_t i = 25; i >= 0; i--){
        float brightness = static_cast<float>(i) * (1.0f/25.0f);
        DebugLeds::SetAllLeds(true, brightness);
        delay(10);
    }
    delay(250);

    // init everything
    gMainControl.init();

    // start everything
    gMainControl.start();
}

void loop() {
    Time::BeginTick();
    gMainControl.tick();
    Time::EndTick();
}
