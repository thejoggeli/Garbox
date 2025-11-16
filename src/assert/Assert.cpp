#include "Assert.h"

#include "AssertHandling.h"

namespace Garbox {

void TriggerDebug(const char* context, const char* message, int32_t arg){
    AssertHandling::InvokeDebug(context, message, arg);
}

void TriggerExit(const char* context, const char* message, int32_t arg){
    AssertHandling::InvokeExit(context, message, arg);
}

void AssertDebug(bool value, const char* context, const char* message, int32_t arg){
    if(!value){
        AssertHandling::InvokeDebug(context, message, arg);
    }
}

void AssertExit(bool value, const char* context, const char* message, int32_t arg){
    if(!value){
        AssertHandling::InvokeExit(context, message, arg);
    }
}

}

