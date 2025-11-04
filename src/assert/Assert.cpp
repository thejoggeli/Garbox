#include "Assert.h"

#include "AssertHandler.h"

namespace Garbox {

void TriggerDebug(const char* context, const char* message) {
    AssertHandler::InvokeDebug(context, message);
}

void TriggerExit(const char* context, const char* message) {
    AssertHandler::InvokeExit(context, message);
}

void AssertDebug(bool value, const char* context, const char* message) {
    if(!value){
        AssertHandler::InvokeDebug(context, message);
    }
}

void AssertExit(bool value, const char* context, const char* message) {
    if(!value){
        AssertHandler::InvokeExit(context, message);
    }
}

}

