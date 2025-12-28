#include "TimeSeries.h"

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
    mSampleIntervalMicros(static_cast<uint64_t>(windowSeconds) * 1000000ull / static_cast<uint64_t>(sampleCount)),
    mSamplingMode(samplingMode),
    mBuffer(sampleCount){
    // constructor body
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

    const uint32_t nowMicros = Time::GetTickMicros();

    // accumulate value if needed
    if(mSamplingMode == SamplingMode::Average){
        mAccumulatedValue += value;
        mAccumulatedCount++;
    }

    const uint32_t elapsedMicros = nowMicros - mLastSampleMicros;

    if((elapsedMicros >= mSampleIntervalMicros) || (mWriteSequence == 0)){

        // write to ring buffer
        if(mSamplingMode == SamplingMode::Average){
            const int32_t averagedValue = mAccumulatedValue / static_cast<int32_t>(mAccumulatedCount);

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

        if(mWriteSequence == 0){
            mLastSampleMicros = nowMicros;
        }
        else {
            mLastSampleMicros += mSampleIntervalMicros;
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
    mTargetWriteSequence(series.mWriteSequence),
    mCurrentWriteSequence(lastSeenWriteSequence){
    // constructor body
    const uint32_t unseenCount = mTargetWriteSequence - lastSeenWriteSequence;
    const uint32_t readableCount = (unseenCount < series.getAvailableSize()) ? unseenCount : series.getAvailableSize();
    if(series.mWriteIndex >= readableCount){
        mReadIndex = series.mWriteIndex - readableCount;
    }
    else {
        mReadIndex = series.getTotalSize() - (readableCount - series.mWriteIndex);
    }
}

bool TimeSeries::IteratorSinceSequence::hasNext() const {
    return (mCurrentWriteSequence < mTargetWriteSequence);
}

int32_t TimeSeries::IteratorSinceSequence::next(){
    AssertDebug((mCurrentWriteSequence < mTargetWriteSequence), "TimeSeries", "iterator exhausted");
    const int32_t value = mSeries.mBuffer[mReadIndex];
    mReadIndex = MathUtils::Wrap<uint16_t>(mReadIndex + 1, mSeries.getTotalSize());
    mCurrentWriteSequence++;
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
