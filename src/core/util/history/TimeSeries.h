#pragma once

#include <cstdint>
#include "core/util/container/heap/ArrayHeap.h"
#include "core/util/container/Span.h"

namespace Garbox {

class TimeSeries {
public:

    enum class SamplingMode : uint8_t {
        Latest = 0,
        Average
    };

    TimeSeries(
        uint32_t windowSeconds, 
        uint16_t sampleCount, 
        SamplingMode samplingMode
    );

    void tick(float value);

    const Span<float> getData() const;
    uint16_t getSize() const;
    uint32_t getTotalUpdatesCount() const;

private:

    // configuration
    const uint32_t mWindowSeconds;
    const uint16_t mSampleCount;
    const uint32_t mSampleIntervalMicros;
    const SamplingMode mSamplingMode;

    // state
    ArrayHeap<float> mBuffer;
    uint16_t mWriteIndex = 0;
    uint32_t mTotalUpdatesCount = 0;
    uint32_t mLastSampleMicros = 0;

    // average sampling mode
    float mAccumulatedValue = 0.0f;
    uint16_t mAccumulatedCount = 0;
};

} // namespace
