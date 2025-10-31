#include "Assert.h"

#include "AssertHandler.h"

namespace Garbox {

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

