#include "AssertHandler.h"

#include "assert/Assert.h"
#include "core/time/Time.h"

namespace Garbox {

static bool sExitTriggered = false;

static AssertHandler::Handler sDebugHandler = nullptr;
static AssertHandler::Handler sExitHandler = nullptr;

void AssertHandler::SetDebugHandler(Handler handler){
    sDebugHandler = handler;
}

void AssertHandler::SetExitHandler(Handler handler){
    sExitHandler = handler;
}

void AssertHandler::InvokeDebug(const char* context, const char* message) {
    if(sExitTriggered){
        return;
    }
    if(sDebugHandler){
        sDebugHandler(context, message);
    }
}

void AssertHandler::InvokeExit(const char* context, const char* message) {
    if(sExitTriggered){
        return;
    }
    sExitTriggered = true;
    if(sExitHandler){
        sExitHandler(context, message);
    }
    while (true) {
        Time::DelayMillis(1000);
    }
}

} // namespace Garbox
