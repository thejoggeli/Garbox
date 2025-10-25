#include "Assert.h"

#include <Arduino.h>
#include "AssertHandler.h"

using Garbox::AssertHandler;

void AssertDebug(bool value, const char* message) {
    if(!value){
        AssertHandler::InvokeDebug(message);
    }
}

void AssertExit(bool value, const char* message) {
    if(!value){
        AssertHandler::InvokeExit(message);
    }
}
