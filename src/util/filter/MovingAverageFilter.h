#pragma once

#include <cstdint>
#include <type_traits>
#include <array>

namespace Garbox {

/**
 * @brief Fixed-size integer FIR moving average filter.
 *
 * Maintains a sliding window of the last N samples and outputs their average.
 * Implements a uniform-coefficient FIR filter using integer arithmetic only.
 */
template <typename T, std::size_t N>
class MovingAverageFilter {

    static_assert(std::is_integral<T>::value, "T must be an integral type");
    static_assert(N > 0, "N must be greater than zero");

public:

    MovingAverageFilter() : 
        // init members
        mCount(0), 
        mIndex(0), 
        mSum(0){
        // nothing to do
    }

    void reset(){
        mCount = 0;
        mIndex = 0;
        mSum = 0;
    }

    void add(T value) {
        if (mCount < N) {
            mValues[mCount++] = value;
            mSum += value;
        } 
        else {
            mSum -= mValues[mIndex];
            mValues[mIndex] = value;
            mSum += value;
            mIndex = (mIndex + 1) % N;
        }
    }

    T getAverage() const {
        if (mCount == 0)
            return static_cast<T>(0);
        return mSum / static_cast<T>(mCount);
    }

    T getSum() const {
        return mSum;
    }

    size_t getCount() const {
        return mCount;
    }

private:

    std::array<T, N> mValues;
    size_t mCount;
    size_t mIndex;
    T mSum;

};

} // namespace