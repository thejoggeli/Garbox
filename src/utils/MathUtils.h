#pragma once

namespace Garbox {

class MathUtils {
public:

    // clamps val to given range [minVal, maxVal]
    template <typename T>
    static T clamp(T val, T minVal, T maxVal) {
        if (val < minVal) return minVal;
        if (val > maxVal) return maxVal;
        return val;
    }

    // maps val from one range [inMin, inMax] to another [outMin, outMax]
    template <typename T>
    static T map(T val, T inMin, T inMax, T outMin, T outMax) {
        if (inMax == inMin) return outMin; // avoid div by zero
        return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

};

}  // namespace Garbox