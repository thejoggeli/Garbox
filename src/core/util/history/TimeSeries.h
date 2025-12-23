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

    void reset();
    void tick(int32_t value);

    const Span<int32_t> getData() const;
    uint16_t getTotalSize() const;
    uint16_t getAvailableSize() const;
    uint32_t getWriteSequence() const;

private:

    // configuration
    const uint32_t mWindowSeconds;
    const uint32_t mSampleIntervalMicros;
    const SamplingMode mSamplingMode;

    // state
    ArrayHeap<int32_t> mBuffer;
    uint16_t mWriteIndex = 0;
    uint16_t mAvailableSize = 0;
    uint32_t mWriteSequence = 0;
    uint32_t mLastSampleMicros = 0;

    // average sampling mode
    int32_t mAccumulatedValue = 0;
    uint16_t mAccumulatedCount = 0;

public:

// ==============================================================================
// IteratorOldestToNewest
// ==============================================================================

    class IteratorOldestToNewest {
    public:
        bool hasNext() const;
        int32_t next();

    private:
        friend class TimeSeries;

        IteratorOldestToNewest(const TimeSeries& series);

        const TimeSeries& mSeries;
        uint16_t mRemaining = 0;
        uint16_t mReadIndex = 0;
    };

// ==============================================================================
// IteratorSinceSequence
// ==============================================================================

    class IteratorSinceSequence {
    public:
        bool hasNext() const;
        int32_t next();
        uint32_t getNewWriteSequence() const;

    private:
        friend class TimeSeries;

        IteratorSinceSequence(
            const TimeSeries& series,
            uint32_t lastSeenWriteSequence
        );

        const TimeSeries& mSeries;
        uint32_t mTargetWriteSequence = 0;
        uint32_t mCurrentWriteSequence = 0;
        uint16_t mReadIndex = 0;
    };

// ==============================================================================
// iterate methods
// ==============================================================================

    IteratorOldestToNewest iterateOldestToNewest() const;
    IteratorSinceSequence iterateSince(uint32_t lastSeenWriteSequence) const;

};

} // namespace
