#pragma once
#include "EasingFunctions.h"

namespace Garbox {

// Wrapper for any easing function with optional parameters
class EasingFunction {
public:

    using FuncPtr = float (*)(float t, const void* p);

    EasingFunction(FuncPtr func = nullptr, const void* params = nullptr);

    float evaluate(float t) const;

    void setFunction(FuncPtr func);
    void setParams(const void* params);

    FuncPtr getFunction() const;
    const void* getParams() const;

private:
    FuncPtr mFunc;
    const void* mParams;
};

} // namespace Garbox
