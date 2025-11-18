#pragma once

#include "core/util/function/math/MathFunctionIfc.h"

namespace Garbox {

/**
 * @brief ScaleFunction
 *
 * Simple proportional scaling function:
 *     y = scale * x
 *
 * Used for efficient unit conversion or normalization without offset.
 */
class ScaleFunction : public MathFunctionIfc {
public:
    ScaleFunction();
    explicit ScaleFunction(float scale);

    void setScale(float scale);
    float getScale() const;

    float evaluate(float x) const final;

private:
    float mScale = 1.0f;
};

} // namespace Garbox
