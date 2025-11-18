#pragma once

#include "core/util/function/FunctionIfc.h"

namespace Garbox {

/**
 * @brief MathFunctionIfc
 *
 * Stateless mathematical function mapping: y = f(x).
 * Serves as a base for analytic, lookup, or sampled functions.
 * 
 * Also implements FunctionIfc, allowing uniform handling of
 * functions and filters in composite processing chains.
 */
class MathFunctionIfc : public FunctionIfc {
public:
    virtual ~MathFunctionIfc() = default;

    virtual float evaluate(float x) const = 0;

    float apply(float value) final {
        return evaluate(value);
    }
};

} // namespace Garbox
