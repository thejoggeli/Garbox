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
        mCountFloat(0.0f), 
        mIndex(0), 
        mSum(0){
        // nothing to do
    }

    void reset(){
        mCount = 0;
        mCountFloat = 0.0f;
        mIndex = 0;
        mSum = 0;
    }

    void add(T value) {
        if (mCount < N) {
            mValues[mCount] = value;
            mCount++;
            mCountFloat = static_cast<float>(mCount);
            mSum += value;
        } 
        else {
            mSum -= mValues[mIndex];
            mValues[mIndex] = value;
            mSum += value;
            mIndex = (mIndex + 1) % N;
        }
    }

    float getAverage() const {
        if (mCount == 0){
            return 0.0f;
        }
        return static_cast<float>(mSum) / mCountFloat;
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
    float mCountFloat;
    size_t mIndex;
    T mSum;

};

} // namespace