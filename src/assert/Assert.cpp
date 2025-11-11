#include "Assert.h"

#include "AssertHandler.h"

namespace Garbox {

void TriggerDebug(const char* context, const char* message, int32_t arg) {
    AssertHandler::InvokeDebug(context, message, arg);
}

void TriggerExit(const char* context, const char* message, int32_t arg) {
    AssertHandler::InvokeExit(context, message, arg);
}

void AssertDebug(bool value, const char* context, const char* message, int32_t arg) {
    if(!value){
        AssertHandler::InvokeDebug(context, message, arg);
    }
}

void AssertExit(bool value, const char* context, const char* message, int32_t arg) {
    if(!value){
        AssertHandler::InvokeExit(context, message, arg);
    }
}

}

