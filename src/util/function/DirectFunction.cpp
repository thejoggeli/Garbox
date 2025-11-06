#include "DirectFunction.h"

#include "assert/Assert.h"

namespace Garbox {

DirectFunction::DirectFunction(){
    // nothing to do
}

DirectFunction::~DirectFunction(){
    // nothing to do
}

void DirectFunction::init(FuncPtr func){
    AssertExit(!mInitialized, "DirectFunction", "already initialized");
    AssertExit(func != nullptr, "DirectFunction", "null function pointer");

    mFunc = func;
    mInitialized = true;
}

float DirectFunction::evaluate(float x) const {
    if(!mInitialized){
        TriggerDebug("DirectFunction", "evaluate() called before init()");
        return 0.0f;
    }

    return mFunc(x);
}

} // namespace Garbox
