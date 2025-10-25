#include <Arduino.h>

#include "assert/AssertHandler.h"
#include "debug/DebugLeds.h"
#include "core/Time.h"
#include "MainControl.h"

using namespace Garbox;

MainControl gMainControl;

void setup() {
    Serial.begin(115200); 

    // init debug leds
    DebugLeds::Init();

    // assert debug handler
    AssertHandler::SetDebugHandler([](const char* message){
        gMainControl.onAssertDebug(message);
    });

    // assert exit handler
    AssertHandler::SetExitHandler([](const char* message){
        gMainControl.onAssertExit(message);
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
