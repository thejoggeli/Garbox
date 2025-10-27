#pragma once
#include <cstdint>
#include <cmath>

namespace Garbox {

/**
 * @brief ExponentialFilter
 * 
 * First-order IIR low-pass filter (exponential smoothing).
 * Formula: y = alpha * target + (1 - alpha) * y
 * 'alpha' controls responsiveness: 1.0 = instant, 0.0 = frozen.
 * Skips updates if change < threshold. Works with any numeric type.
 */
template <typename T>
class ExponentialFilter {
public:

    /// Initialize with user-defined alpha value
    /// @param alpha 
    /// @param threshold 
    explicit ExponentialFilter(float alpha, float threshold = 0.5f) : 
        // init members 
        mAlpha(clampAlpha(alpha)),
        mThreshold(threshold),
        mCurrentValue(static_cast<T>(0)),
        mCurrentValueFloat(0.0f){
        // nothing to do
    }

    /// Choose alpha such that targetFraction of targetValue is reached within a set number ticks
    /// @param targetFraction 
    /// @param ticks 
    /// @param threshold 
    ExponentialFilter(float targetFraction, uint32_t ticks, float threshold = 0.5f) : 
        // init members 
        mAlpha(computeAlpha(targetFraction, ticks)),
        mThreshold(threshold),
        mCurrentValue(static_cast<T>(0)),
        mCurrentValueFloat(0.0f){
        // nothing to do
    }

    void setCurrentValue(T value) {
        mCurrentValue = value;
        mCurrentValueFloat = static_cast<float>(value);
    }

    T update(T targetValue) {
        float target = static_cast<float>(targetValue);
        float diff = std::fabs(target - mCurrentValueFloat);

        if (diff < mThreshold)
            return mCurrentValue;

        mCurrentValueFloat = mAlpha * target + (1.0f - mAlpha) * mCurrentValueFloat;

        T rounded = static_cast<T>(mCurrentValueFloat);
        if (std::fabs(mCurrentValueFloat - static_cast<float>(rounded)) < mThreshold) {
            mCurrentValue = rounded;
            mCurrentValueFloat = static_cast<float>(mCurrentValue);
        }

        return mCurrentValue;
    }

    T getCurrentValue() const { return mCurrentValue; }
    float getCurrentValueFloat() const { return mCurrentValueFloat; }
    float getAlpha() const { return mAlpha; }
    float getThreshold() const { return mThreshold; }

    void setAlpha(float alpha) { mAlpha = clampAlpha(alpha); }
    void setThreshold(float threshold) { mThreshold = threshold; }

    static float computeAlpha(float targetFraction, uint32_t ticks) {
        if (ticks == 0)
            return 1.0f;
        if (targetFraction <= 0.0f)
            return 0.0f;
        if (targetFraction >= 1.0f)
            return 1.0f;

        float remaining = 1.0f - targetFraction;
        return 1.0f - std::pow(remaining, 1.0f / static_cast<float>(ticks));
    }

private:

    static float clampAlpha(float alpha) {
        if (alpha < 0.0f) return 0.0f;
        if (alpha > 1.0f) return 1.0f;
        return alpha;
    }

    float mAlpha;
    float mThreshold;
    float mCurrentValueFloat;
    T mCurrentValue;

};

} // namespace