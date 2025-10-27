#pragma once

namespace Garbox {

class MathUtils {
public:

    /// clamps val to given range [minVal, maxVal]
    template <typename T>
    static T clamp(T val, T minVal, T maxVal) {
        if (val < minVal) return minVal;
        if (val > maxVal) return maxVal;
        return val;
    }

    /// maps val from one range [inMin, inMax] to another [outMin, outMax]
    template <typename T>
    static T map(T val, T inMin, T inMax, T outMin, T outMax) {
        if (inMax == inMin) return outMin; // avoid div by zero
        return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    /// exponential smoothing
    static float exponentialSmoothing(float currentValue, float targetValue, float alpha){
        if (alpha <= 0.0f)
            return currentValue; // no update

        if (alpha >= 1.0f)
            return targetValue; // immediate response
    
        return alpha * targetValue + (1.0f - alpha) * currentValue;
    }

    /// helper for exponential smoothing to reach fraction in a given amount of ticks
    /// targetFraction: desired fraction of target reached (e.g. 0.99 for 99%)
    /// ticks: number of discrete updates to reach that level
    static float computeAlpha(float targetFraction, uint32_t ticks){
        if (ticks == 0)
            return 1.0f; // immediate update, no smoothing

        if (targetFraction <= 0.0f)
            return 0.0f; // would never move

        if (targetFraction >= 1.0f)
            return 1.0f; // theoretically infinite ticks; clamp

        float remaining = 1.0f - targetFraction;
        return 1.0f - powf(remaining, 1.0f / static_cast<float>(ticks));
    }

};

}  // namespace Garbox