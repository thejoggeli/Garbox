#pragma once

#include "core/util/function/filter/FilterFunctionAbs.h"
#include <cstdint>
#include <cstddef>

namespace Garbox {

/**
 * @brief MovingAverageFilter
 *
 * Fixed-point moving average filter.
 * Maintains a ring buffer of scaled integer samples and computes their average.
 *
 * Scaling:
 *   scaledValue = input * scaleFactor
 *   output = average(scaledValue) / scaleFactor
 *
 * Two initialization modes:
 * - External buffer (no heap usage)
 * - Heap-based (allocates once during construction)
 *
 * The user is responsible for choosing a scale factor large enough
 * to preserve resolution and small enough to avoid overflow.
 */
class MovingAverageFilter : public FilterFunctionAbs {
public:
    // external buffer constructor
    MovingAverageFilter(int32_t* buffer, uint32_t size, float scaleFactor = 1000.0f);

    // heap-based constructor
    MovingAverageFilter(uint32_t size, float scaleFactor = 1000.0f);

    ~MovingAverageFilter();

    void setScaleFactor(float scaleFactor);

protected:
    void onReset(float value) override;
    float onProcess(float value) override;

private:
    int32_t* mBuffer = nullptr;
    uint32_t mSize = 0;
    uint32_t mIndex = 0;
    uint32_t mCount = 0;
    int32_t mSum = 0;
    float mCountFloat = 0.0f;
    float mScaleFactor = 1000.0f;
    bool mHeapAllocated = false;
};

} // namespace Garbox
