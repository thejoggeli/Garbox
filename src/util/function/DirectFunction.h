#pragma once

#include <cstdint>
#include "FunctionIfc.h"

namespace Garbox {

class DirectFunction : public FunctionIfc {
public:
    using FuncPtr = float (*)(float x);

    DirectFunction();
    ~DirectFunction();

    // Initialize the function
    void init(FuncPtr func);

    // Evaluate the wrapped function
    float evaluate(float x) const final;

private:
    FuncPtr mFunc = nullptr;
    bool mInitialized = false;
};

} // namespace Garbox
