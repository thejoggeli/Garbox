#pragma once
#include <cstdint>

namespace Garbox {

/**
 * @brief ExponentialFilter (float-only)
 * 
 * First-order IIR low-pass filter (exponential smoothing).
 * Formula: y = alpha * target + (1 - alpha) * current
 * 'alpha' controls responsiveness: 1.0 = instant, 0.0 = frozen.
 * Skips updates if change < threshold.
 */
class ExponentialFilter {
public:
    ExponentialFilter();
    ExponentialFilter(float alpha, float threshold);

    void update(float targetValue);
    void setFilteredValue(float value);

    float getFilteredValue() const;
    float getAlpha() const;
    float getThreshold() const;

    void setAlpha(float alpha);
    void setAlphaComputed(float fraction, uint32_t ticks);
    void setThreshold(float threshold);

    static float computeAlpha(float fraction, uint32_t ticks);

private:
    static float clampAlpha(float alpha);

    float mAlpha = 0.9f;
    float mThreshold = 0.01f;
    float mFilteredValue = 0.0f;
};

} // namespace Garbox
