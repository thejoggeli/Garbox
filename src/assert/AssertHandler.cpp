#include "AssertHandler.h"

#include <Arduino.h>

namespace Garbox {

AssertHandler::Handler AssertHandler::sDebugHandler = nullptr;
AssertHandler::Handler AssertHandler::sExitHandler = nullptr;

void AssertHandler::SetDebugHandler(Handler handler){
    sDebugHandler = handler;
}

void AssertHandler::SetExitHandler(Handler handler){
    sExitHandler = handler;
}

void AssertHandler::InvokeDebug(const char* message) {
    if(sDebugHandler){
        sDebugHandler(message);
    }
}

void AssertHandler::InvokeExit(const char* message) {
    if(sExitHandler){
        sExitHandler(message);
    }
    while (true) {
        delay(1000);
    }
}

} // namespace Garbox
