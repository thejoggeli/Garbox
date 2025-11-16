#include "assert/AssertHandling.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace Garbox {

static AssertHandling::Handler gDebugHandler = nullptr;
static AssertHandling::Handler gExitHandler = nullptr;

void AssertHandling::SetDebugHandler(Handler handler){
    gDebugHandler = handler;
}

void AssertHandling::SetExitHandler(Handler handler){
    gExitHandler = handler;
}

void AssertHandling::InvokeDebug(const char* context, const char* message, int32_t arg){
    printf("[ASSERT-DEBUG] %s: %s (arg=%u)\n", context, message, arg);
    if(gDebugHandler){
        gDebugHandler(context, message, arg);
    }
}

void AssertHandling::InvokeExit(const char* context, const char* message, int32_t arg){
    printf("[ASSERT-EXIT] %s: %s (arg=%u)\n", context, message, arg);
    if(gExitHandler){
        gExitHandler(context, message, arg);
    }
}

} // namespace Garbox
