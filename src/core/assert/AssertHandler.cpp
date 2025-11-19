#include "AssertHandler.h"

#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "core/rtos/LockGuard.h"

namespace Garbox {

// exit on assert debug
static constexpr bool DebugTriggersExit = true;

// exit already triggered flag to prevent infinite recursion if a handler triggers another assert
static bool gExitTriggered = false;

// custom handlers
static AssertHandler::Handler gDebugHandler = nullptr;
static AssertHandler::Handler gExitHandler = nullptr;

// handler mutex
static SemaphoreHandle_t gAssertDebugHandlerMutex = nullptr;
static SemaphoreHandle_t gAssertExitHandlerMutex = nullptr;

// set debug handler
void AssertHandler::SetDebugHandler(Handler handler){
    gDebugHandler = handler;
}

// set exit handler
void AssertHandler::SetExitHandler(Handler handler){
    gExitHandler = handler;
}

void AssertHandler::InvokeDebug(const char* context, const char* message, int32_t arg){

    // abort if exit already triggerd to prevent infinite recursive calls
    if(gExitTriggered){
        LogError("AssertHandler", "AssertDebug! %s %s (arg=%" PRIi32 ")", context, message, arg);
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
        LogError("AssertHandler", "AssertDebug! %s %s (arg=%" PRIi32 ")", context, message, arg);
    }
}

void AssertHandler::InvokeExit(const char* context, const char* message, int32_t arg){

    // abort if exit already triggerd to prevent infinite recursive calls
    if(gExitTriggered){
        LogError("AssertHandler", "AssertExit! %s %s (arg=%" PRIi32 ")", context, message, arg);
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
        LogError("AssertHandler", "AssertExit! %s %s (arg=%" PRIi32 ")", context, message, arg);
        Time::BlockMillis(1000);
    }
}

} // namespace Garbox
