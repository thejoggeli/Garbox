#include "AnalyticFunction.h"

#include "assert/Assert.h"

namespace Garbox {

AnalyticFunction::AnalyticFunction(){
    // nothing to do
}

void AnalyticFunction::init(FuncPtr func){
    AssertExit(!mInitialized, "AnalyticFunction", "already initialized");
    AssertExit(func != nullptr, "AnalyticFunction", "null function pointer");

    mFunc = func;
    mInitialized = true;
}

float AnalyticFunction::evaluate(float x) const {
    if(!mInitialized){
        TriggerDebug("AnalyticFunction", "evaluate() called before init()");
        return 0.0f;
    }

    return mFunc(x);
}

} // namespace Garbox
