#pragma once

#include <cstdint>
#include "util/transform/function/FunctionIfc.h"

namespace Garbox {

/**
 * @brief Wrapper for a user-supplied analytic function pointer.
 *
 * Calls a provided function pointer (float => float) directly during evaluation.
 * This allows embedding lightweight analytic expressions without creating a subclass.
 */
class AnalyticFunction : public FunctionIfc {
public:
    using FuncPtr = float (*)(float x);

    AnalyticFunction();

    // Initialize the function
    void init(FuncPtr func);

    // Evaluate the wrapped function
    float evaluate(float x) const final;

private:
    FuncPtr mFunc = nullptr;
    bool mInitialized = false;
};

} // namespace Garbox
