#include "TimeSeries.h"

#include <limits>
#include <new>

#include "core/assert/Assert.h"
#include "core/time/Time.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

TimeSeries::TimeSeries(
    uint32_t windowSeconds,
    uint16_t sampleCount,
    SamplingMode samplingMode):
    // initialize members
    mWindowSeconds(windowSeconds),
    mSampleIntervalMicros(
        (windowSeconds > 0 && sampleCount > 0)
            ? (static_cast<uint64_t>(windowSeconds) * 1000000ULL / sampleCount)
            : 1ULL
    ),
    mSamplingMode(samplingMode),
    mBuffer(sampleCount > 0 ? sampleCount : 1){
    // constructor body
    AssertExit(windowSeconds > 0, "TimeSeries", "window must be positive");
    AssertExit(sampleCount > 0, "TimeSeries", "sample count must be positive");
}

void TimeSeries::reset(){
    mWriteIndex = 0;
    mAvailableSize = 0;
    mWriteSequence = 0;
    mLastSampleMicros = 0;
    mAccumulatedValue = 0;
    mAccumulatedCount = 0;
}

void TimeSeries::tick(int32_t value){

    const uint64_t nowMicros = Time::GetTickMicros64();

    // accumulate value if needed
    if(mSamplingMode == SamplingMode::Average){
        const bool sumWouldOverflow =
            (value > 0 && mAccumulatedValue > std::numeric_limits<int64_t>::max() - value)
            || (value < 0 && mAccumulatedValue < std::numeric_limits<int64_t>::min() - value);
        const bool countWouldOverflow = mAccumulatedCount == std::numeric_limits<uint64_t>::max();
        if(sumWouldOverflow || countWouldOverflow){
            TriggerExit("TimeSeries", "average accumulator overflow");
            mAccumulatedValue = 0;
            mAccumulatedCount = 0;
        }
        mAccumulatedValue += value;
        mAccumulatedCount++;
    }

    const uint64_t elapsedMicros = nowMicros - mLastSampleMicros;
    const bool firstSample = (mAvailableSize == 0);

    if((elapsedMicros >= mSampleIntervalMicros) || firstSample){

        // write to ring buffer
        if(mSamplingMode == SamplingMode::Average){
            const int32_t averagedValue = static_cast<int32_t>(
                mAccumulatedValue / static_cast<int64_t>(mAccumulatedCount)
            );

            mBuffer[mWriteIndex] = averagedValue;
            mAccumulatedValue = 0;
            mAccumulatedCount = 0;
        }
        else {
            mBuffer[mWriteIndex] = value;
        }

        // update state
        mWriteIndex = MathUtils::Wrap<uint16_t>(mWriteIndex + 1, mBuffer.size());

        if(mAvailableSize < mBuffer.size()){
            mAvailableSize++;
        }

        if(firstSample){
            mLastSampleMicros = nowMicros;
        }
        else {
            const uint64_t elapsedPeriods = elapsedMicros / mSampleIntervalMicros;
            mLastSampleMicros += elapsedPeriods * mSampleIntervalMicros;
        }

        mWriteSequence++;
    }
}

const Span<int32_t> TimeSeries::getData() const {
    return Span<int32_t>(
        const_cast<int32_t*>(mBuffer.data()),
        static_cast<size_t>(mAvailableSize)
    );
}

uint16_t TimeSeries::getTotalSize() const {
    return mBuffer.size();
}

uint16_t TimeSeries::getAvailableSize() const {
    return mAvailableSize;
}

uint32_t TimeSeries::getWriteSequence() const {
    return mWriteSequence;
}

uint64_t TimeSeries::getSampleIntervalMicros() const {
    return mSampleIntervalMicros;
}

// ==============================================================================
// IteratorOldestToNewest
// ==============================================================================

TimeSeries::IteratorOldestToNewest::IteratorOldestToNewest(const TimeSeries& series):
    // initialize members
    mSeries(series),
    mRemaining(series.mAvailableSize){
    // constructor body
    if(mSeries.getAvailableSize() != mSeries.getTotalSize()){
        mReadIndex = 0;
    }
    else {
        mReadIndex = mSeries.mWriteIndex;
    }
}

bool TimeSeries::IteratorOldestToNewest::hasNext() const {
    return (mRemaining > 0);
}

int32_t TimeSeries::IteratorOldestToNewest::next(){
    AssertDebug((mRemaining > 0), "TimeSeries", "iterator exhausted");
    const int32_t value = mSeries.mBuffer[mReadIndex];
    mReadIndex = MathUtils::Wrap<uint16_t>(mReadIndex + 1, mSeries.getTotalSize());
    mRemaining--;
    return value;
}

// ==============================================================================
// IteratorSinceSequence
// ==============================================================================

TimeSeries::IteratorSinceSequence::IteratorSinceSequence(const TimeSeries& series, uint32_t lastSeenWriteSequence):
    // initialize members
    mSeries(series),
    mTargetWriteSequence(series.mWriteSequence){
    // constructor body
    const uint32_t unseenCount = mTargetWriteSequence - lastSeenWriteSequence;
    const uint32_t readableCount = (unseenCount < series.getAvailableSize()) ? unseenCount : series.getAvailableSize();
    mRemaining = static_cast<uint16_t>(readableCount);
    if(series.mWriteIndex >= readableCount){
        mReadIndex = series.mWriteIndex - readableCount;
    }
    else {
        mReadIndex = series.getTotalSize() - (readableCount - series.mWriteIndex);
    }
}

bool TimeSeries::IteratorSinceSequence::hasNext() const {
    return (mRemaining > 0);
}

int32_t TimeSeries::IteratorSinceSequence::next(){
    AssertDebug((mRemaining > 0), "TimeSeries", "iterator exhausted");
    if(mRemaining == 0) return 0;
    const int32_t value = mSeries.mBuffer[mReadIndex];
    mReadIndex = MathUtils::Wrap<uint16_t>(mReadIndex + 1, mSeries.getTotalSize());
    mRemaining--;
    return value;
}

uint32_t TimeSeries::IteratorSinceSequence::getNewWriteSequence() const {
    return mTargetWriteSequence;
}

// ==============================================================================
// iterate methods
// ==============================================================================

TimeSeries::IteratorOldestToNewest TimeSeries::iterateOldestToNewest() const {
    return IteratorOldestToNewest(*this);
}

TimeSeries::IteratorSinceSequence TimeSeries::iterateSince(uint32_t lastSeenWriteSequence) const {
    return IteratorSinceSequence(*this, lastSeenWriteSequence);
}

} // namespace
