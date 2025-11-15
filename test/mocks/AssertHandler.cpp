#include "assert/AssertHandler.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace Garbox {

static AssertHandler::Handler sDebugHandler = nullptr;
static AssertHandler::Handler sExitHandler = nullptr;

void AssertHandler::SetDebugHandler(Handler handler){
    sDebugHandler = handler;
}

void AssertHandler::SetExitHandler(Handler handler){
    sExitHandler = handler;
}

void AssertHandler::InvokeDebug(const char* context, const char* message, int32_t arg) {
    printf("[ASSERT-DEBUG] %s: %s (arg=%u)\n", context, message, arg);
    if(sDebugHandler){
        sDebugHandler(context, message, arg);
    }
}

void AssertHandler::InvokeExit(const char* context, const char* message, int32_t arg) {
    printf("[ASSERT-EXIT] %s: %s (arg=%u)\n", context, message, arg);
    if(sExitHandler){
        sExitHandler(context, message, arg);
    }
}

} // namespace Garbox
