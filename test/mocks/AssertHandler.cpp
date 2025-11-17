#include "assert/AssertHandler.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace Garbox {

static AssertHandler::Handler gDebugHandler = nullptr;
static AssertHandler::Handler gExitHandler = nullptr;

void AssertHandler::SetDebugHandler(Handler handler){
    gDebugHandler = handler;
}

void AssertHandler::SetExitHandler(Handler handler){
    gExitHandler = handler;
}

void AssertHandler::InvokeDebug(const char* context, const char* message, int32_t arg){
    printf("[ASSERT-DEBUG] %s: %s (arg=%u)\n", context, message, arg);
    if(gDebugHandler){
        gDebugHandler(context, message, arg);
    }
}

void AssertHandler::InvokeExit(const char* context, const char* message, int32_t arg){
    printf("[ASSERT-EXIT] %s: %s (arg=%u)\n", context, message, arg);
    if(gExitHandler){
        gExitHandler(context, message, arg);
    }
}

} // namespace Garbox
