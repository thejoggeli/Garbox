#include <Arduino.h>

#include "assert/AssertHandler.h"
#include "debug/DebugLeds.h"
#include "core/Time.h"
#include "control/MainControl.h"

using namespace Garbox;

MainControl gMainControl;

void setup() {
    Serial.begin(115200); 

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
