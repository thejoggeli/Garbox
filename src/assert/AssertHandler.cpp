#include "AssertHandler.h"

#include "core/time/Time.h"

namespace Garbox {

AssertHandler::Handler AssertHandler::sDebugHandler = nullptr;
AssertHandler::Handler AssertHandler::sExitHandler = nullptr;

void AssertHandler::SetDebugHandler(Handler handler){
    sDebugHandler = handler;
}

void AssertHandler::SetExitHandler(Handler handler){
    sExitHandler = handler;
}

void AssertHandler::InvokeDebug(const char* context, const char* message) {
    if(sDebugHandler){
        sDebugHandler(context, message);
    }
}

void AssertHandler::InvokeExit(const char* context, const char* message) {
    if(sExitHandler){
        sExitHandler(context, message);
    }
    while (true) {
        Time::DelayMillis(1000);
    }
}

} // namespace Garbox
