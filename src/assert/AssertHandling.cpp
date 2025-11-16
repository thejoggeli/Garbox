#include "AssertHandling.h"

#include "assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "util/threading/LockGuard.h"

namespace Garbox {

// exit on assert debug
static constexpr bool DebugTriggersExit = true;

// exit already triggered flag to prevent infinite recursion if a handler triggers another assert
static bool gExitTriggered = false;

// custom handlers
static AssertHandling::Handler gDebugHandler = nullptr;
static AssertHandling::Handler gExitHandler = nullptr;

// handler mutex
static SemaphoreHandle_t gAssertDebugHandlerMutex = nullptr;
static SemaphoreHandle_t gAssertExitHandlerMutex = nullptr;

// set debug handler
void AssertHandling::SetDebugHandler(Handler handler){
    gDebugHandler = handler;
}

// set exit handler
void AssertHandling::SetExitHandler(Handler handler){
    gExitHandler = handler;
}

void AssertHandling::InvokeDebug(const char* context, const char* message, int32_t arg){

    // abort if exit already triggerd to prevent infinite recursive calls
    if(gExitTriggered){
        return;
    }

    // check if forward to exit is enabled
    if(DebugTriggersExit){
        InvokeExit(context, message, arg);
        return;
    }

    // invoke debug handler
    if(gDebugHandler){
        LockGuard lock(gAssertDebugHandlerMutex);
        gDebugHandler(context, message, arg);
    }
    else {
        LogError("AssertHandling", "AssertDebug! %s %s (arg=%" PRIi32 ")", context, message, arg);
    }
}

void AssertHandling::InvokeExit(const char* context, const char* message, int32_t arg){

    // abort if exit already triggerd to prevent infinite recursive calls
    if(gExitTriggered){
        return;
    }
    gExitTriggered = true;

    // call exit handler
    if(gExitHandler){
        LockGuard lock(gAssertExitHandlerMutex);
        gExitHandler(context, message, arg);
    }

    // default exit handler
    while (true){
        LogError("AssertHandling", "AssertExit! %s %s (arg=%" PRIi32 ")", context, message, arg);
        Time::DelayMillis(1000);
    }
}

} // namespace Garbox
