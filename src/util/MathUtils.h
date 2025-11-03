#pragma once

#include <cstdint>
#include <type_traits>

namespace Garbox {

class MathUtils {
public:

    /// clamps val to given range [minVal, maxVal]
    template <typename T>
    static T Clamp(T val, T minVal, T maxVal) {
        if (val < minVal) return minVal;
        if (val > maxVal) return maxVal;
        return val;
    }

    /// maps val from one range [inMin, inMax] to another [outMin, outMax]
    template <typename T>
    static T Map(T val, T inMin, T inMax, T outMin, T outMax) {
        if (inMax == inMin) return outMin; // avoid div by zero
        return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    // unified wrap function 
    // exclusive endpoint: output will be in the range [a, b)
    // supported types: unsigned int, signed int, float, double
    template <typename T>
    static T Wrap(T value, T a, T b = 0) {
        if constexpr (std::is_unsigned_v<T>) {
            // unsigned wrap within [0, a)
            T wrap = a;
            if (wrap == 0) return 0;
            while (value >= wrap)
                value -= wrap;
            return value;

        } 
        else if constexpr (std::is_signed_v<T> || std::is_floating_point_v<T>) {
            // signed or floating-point wraps within [a, b)
            T minVal = a;
            T maxVal = b;

            if (minVal >= maxVal) return minVal;
            T range = maxVal - minVal;

            while (value >= maxVal)
                value -= range;
            while (value < minVal)
                value += range;

            return value;

        } 
        else {
            static_assert(std::is_arithmetic_v<T>, "Wrap supports only numeric types");
            return value;
        }
    }

};

}  // namespace Garbox