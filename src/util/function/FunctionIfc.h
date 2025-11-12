#pragma once

#include <cstdint>

namespace Garbox {

/**
 * @brief FunctionIfc
 *
 * Minimal interface for mathematical transformations.
 * Provides a unified API for stateless and stateful processing.
 *
 * Example implementations:
 * - MathFunctionIfc (pure mathematical mappings)
 * - FilterFunctionIfc (stateful signal filtering)
 */
class FunctionIfc {
public:
    virtual ~FunctionIfc() = default;
    virtual float apply(float value) = 0;
};

} // namespace Garbox
