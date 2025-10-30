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
    explicit ExponentialFilter(float alpha, float threshold = 0.1f);
    ExponentialFilter(float fraction, uint32_t ticks, float threshold = 0.1f);

    void setCurrentValue(float value);
    void update(float targetValue);

    float getCurrentValue() const;
    float getAlpha() const;
    float getThreshold() const;

    void setAlpha(float alpha);
    void setThreshold(float threshold);

    static float computeAlpha(float fraction, uint32_t ticks);

private:
    static float clampAlpha(float alpha);

    float mAlpha;
    float mThreshold;
    float mCurrentValue;
};

} // namespace Garbox
