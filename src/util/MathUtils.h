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

    // returns the absolute difference between two numeric values
    // works for both signed and unsigned types; avoids undefined behavior for unsigned arithmetic
    template <typename T>
    static constexpr T AbsDiff(T a, T b) noexcept {
        static_assert(std::is_arithmetic_v<T>, "AbsDiff supports only numeric types");
        if constexpr (std::is_unsigned_v<T>) {
            return (a > b) ? (a - b) : (b - a);
        } 
        else if constexpr (std::is_signed_v<T> || std::is_floating_point_v<T>) {
            return (a >= b) ? (a - b) : (b - a);
        }
    }

    // wraps value into the range [0, max) without using modulo
    // this function is meant to be used with small changes to value only. it handles at most one overflow
    template <typename T>
    static T Wrap(T value, T max) {
        if (max == 0) return 0;
        if constexpr (std::is_unsigned_v<T>) {
            if (value >= max)
                value -= max;
            return value;
        } 
        else if constexpr (std::is_signed_v<T> || std::is_floating_point_v<T>) {
            if (value >= max)
                value -= max;
            else if (value < 0)
                value += max;
            return value;
        } 
        static_assert(std::is_arithmetic_v<T>, "Wrap supports only numeric types");
    }

    // wraps value into the range [min, max) without using modulo
    // this function is meant to be used with small changes to value only. it handles at most one over/underflow
    template <typename T>
    static T Wrap(T value, T min, T max) {
        if (min >= max) return min;
        T range = max - min;
        if constexpr (std::is_unsigned_v<T>) {
            if (value >= max)
                value -= range;
            else if (value < min)
                value += range;
            return value;
        } 
        else if constexpr (std::is_signed_v<T> || std::is_floating_point_v<T>) {
            if (value >= max)
                value -= range;
            else if (value < min)
                value += range;
            return value;
        } 
        static_assert(std::is_arithmetic_v<T>, "Wrap supports only numeric types");
    }

};

}  // namespace Garbox