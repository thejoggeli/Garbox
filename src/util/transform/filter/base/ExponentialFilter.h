#pragma once

#include "util/transform/filter/FilterAbs.h"
#include <cmath>

namespace Garbox {

/**
 * @brief ExponentialFilter
 *
 * First-order low-pass filter using exponential smoothing.
 * Formula: y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
 *
 * - 'alpha' controls responsiveness (1.0 = instant, 0.0 = frozen)
 * - 'threshold' defines a minimum delta required to update (optional deadband)
 */
class ExponentialFilter : public FilterAbs {
public:
    ExponentialFilter();
    ExponentialFilter(float alpha, float threshold = 0.001f);

    void setAlpha(float alpha);
    void setAlphaComputed(float fraction, uint32_t ticks);
    void setThreshold(float threshold);

    float getAlpha() const;
    float getThreshold() const;

protected:
    float onProcess(float value) override;

private:
    static float computeAlpha(float fraction, uint32_t ticks);

    float mThreshold = 0.001f;
    float mAlpha = 0.9f;
    float mOneMinusAlpha;
};

} // namespace Garbox
