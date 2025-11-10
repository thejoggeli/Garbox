#pragma once

#include <cstdint>

namespace Garbox {

/**
 * @brief TransformIfc
 *
 * Minimal interface for mathematical transformations.
 * Provides a unified API for stateless and stateful processing.
 *
 * Example implementations:
 * - FunctionIfc (pure mathematical mappings)
 * - FilterIfc (stateful signal filtering)
 */
class TransformIfc {
public:
    virtual ~TransformIfc() = default;
    virtual float apply(float value) = 0;
};

} // namespace Garbox
