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
    mSampleCount(sampleCount),
    mSampleIntervalMicros((windowSeconds * 1000000UL) / sampleCount),
    mSamplingMode(samplingMode),
    mBuffer(sampleCount){}

void TimeSeries::reset(){
    mWriteIndex = 0;
    mTotalUpdatesCount = 0;
    mLastSampleMicros = 0;
    mAccumulatedValue = 0.0f;
    mAccumulatedCount = 0;
}

void TimeSeries::tick(float value){

    uint32_t nowMicros = Time::GetTickMicros();

    // accumulate value
    if(mSamplingMode == SamplingMode::Average){
        mAccumulatedValue += value;
        mAccumulatedCount++;
    }

    const uint32_t elapsedMicros = nowMicros - mLastSampleMicros;

    if(elapsedMicros >= mSampleIntervalMicros || mTotalUpdatesCount == 0){

        // write to ring buffer
        if(mSamplingMode == SamplingMode::Average){
            const float averagedValue = mAccumulatedValue / static_cast<float>(mAccumulatedCount);
            mBuffer[mWriteIndex] = averagedValue;
            mAccumulatedValue = 0.0f;
            mAccumulatedCount = 0;
        }
        else {
            mBuffer[mWriteIndex] = value;
        }

        // update state
        mWriteIndex = MathUtils::Wrap<uint16_t>(mWriteIndex + 1, mSampleCount);
        if(mTotalUpdatesCount == 0){
            mLastSampleMicros = nowMicros;
        }
        else {
            mLastSampleMicros += mSampleIntervalMicros;
        }
        mTotalUpdatesCount++;
    }
}

const Span<float> TimeSeries::getData() const {
    return Span<float>(const_cast<float*>(mBuffer.data()), static_cast<size_t>(mAccumulatedCount));
}

uint16_t TimeSeries::getSize() const{
    return mSampleCount;
}

uint32_t TimeSeries::getTotalUpdatesCount() const{
    return mTotalUpdatesCount;
}

} // namespace
