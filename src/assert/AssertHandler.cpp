#include "AssertHandler.h"

#include "assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"

namespace Garbox {

static constexpr bool DebugTriggersExit = true;

static bool sExitTriggered = false;

static AssertHandler::Handler sDebugHandler = nullptr;
static AssertHandler::Handler sExitHandler = nullptr;

void AssertHandler::SetDebugHandler(Handler handler){
    sDebugHandler = handler;
}

void AssertHandler::SetExitHandler(Handler handler){
    sExitHandler = handler;
}

void AssertHandler::InvokeDebug(const char* context, const char* message, int32_t arg) {
    if(sExitTriggered){
        return;
    }

    // check if forward to exit is enabled
    if(DebugTriggersExit){
        InvokeExit(context, message, arg);
        return;
    }

    // invoke debug handler
    if(sDebugHandler){
        sDebugHandler(context, message, arg);
    }
    else {
        LogError("AssertHandler", "AssertDebug! %s %s", context, message);
    }
}

void AssertHandler::InvokeExit(const char* context, const char* message, int32_t arg) {
    if(sExitTriggered){
        return;
    }
    sExitTriggered = true;

    // call exit handler
    if(sExitHandler){
        sExitHandler(context, message, arg);
    }

    // default exit handler
    while (true) {
        LogError("AssertHandler", "AssertExit! %s %s (arg=%" PRIi32 ")", context, message, arg);
        Time::DelayMillis(1000);
    }
}

} // namespace Garbox
