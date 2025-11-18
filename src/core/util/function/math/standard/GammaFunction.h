#pragma once

#include "core/util/function/math/MathFunctionIfc.h"

namespace Garbox {

/**
 * @brief GammaFunction
 *
 * Applies a gamma correction curve:
 *     y = powf(x, gamma)
 *
 * Often used for non-linear mapping of input signals,
 * e.g. brightness, color correction, or perceptual scaling.
 */
class GammaFunction : public MathFunctionIfc {
public:
    GammaFunction(float gamma = 1.0f);
    ~GammaFunction() = default;

    void setGamma(float gamma); // set gamma exponent (must be > 0)
    float evaluate(float x) const final;
    float getGamma() const;

private:
    float mGamma = 1.0f;
};

} // namespace Garbox
