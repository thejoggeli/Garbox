#include "EasingFunction.h"

namespace Garbox {

EasingFunction::EasingFunction(FuncPtr func, const void* params) : mFunc(func), mParams(params) {
    // nothing to do
}

float EasingFunction::evaluate(float t) const {
    if (!mFunc)
        return t; // fallback: linear passthrough if function not set
    return mFunc(t, mParams);
}

void EasingFunction::setFunction(FuncPtr func) {
    mFunc = func;
}

void EasingFunction::setParams(const void* params) {
    mParams = params;
}

EasingFunction::FuncPtr EasingFunction::getFunction() const {
    return mFunc;
}

const void* EasingFunction::getParams() const {
    return mParams;
}

} // namespace Garbox
