#pragma once

#include "util/transform/TransformIfc.h"

namespace Garbox {

/**
 * @brief FunctionIfc
 *
 * Stateless mathematical function mapping: y = f(x).
 * Serves as a base for analytic, lookup, or sampled functions.
 * 
 * Also implements TransformIfc, allowing uniform handling of
 * functions and filters in composite processing chains.
 */
class FunctionIfc : public TransformIfc {
public:
    virtual ~FunctionIfc() = default;

    virtual float evaluate(float x) const = 0;

    float apply(float value) final {
        return evaluate(value);
    }
};

} // namespace Garbox
