#pragma once

#include <cstdint>
#include "core/util/function/math/MathFunctionIfc.h"

namespace Garbox {

/**
 * @brief Simple constant-valued function.
 *
 * Always returns a fixed value regardless of input x.
 * Useful as a placeholder, default source, or in composite function chains.
 */
class ConstantFunction : public MathFunctionIfc {
public:

    ConstantFunction(float value);

    // Evaluate the wrapped function
    float evaluate(float x) const final;

private:
    float mValue;
};

} // namespace Garbox
