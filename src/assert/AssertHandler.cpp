#include "AssertHandler.h"

#include <Arduino.h>

namespace Garbox {

void AssertHandler::Init(){
    // nothing to do
}

void AssertHandler::Start(){
    // nothing to do
}

void AssertHandler::InvokeDebug(const char* message) {
    Serial.println("ASSERT_DEBUG triggered:");
    Serial.println(message);
    // TODO turn on assert debug LED
}

void AssertHandler::InvokeExit(const char* message) {
    Serial.println("ASSERT_EXIT triggered:");
    Serial.println(message);
    // TODO turn on all debug LEDs
    while (true) {
        delay(1000);
    }
}

} // namespace Garbox
