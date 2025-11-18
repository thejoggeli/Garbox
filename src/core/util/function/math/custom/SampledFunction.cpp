#include "SampledFunction.h"

#include <cmath>
#include <cstring>
#include "core/assert/Assert.h"

namespace Garbox {

SampledFunction::SampledFunction(){
    // nothing to do
}

SampledFunction::~SampledFunction(){
    if(mHeapAllocated){
        // runtime destruction of heap-based lookup tables is not allowed
        TriggerExit("SampledFunction", "heap using classes must not be deconstructed");
    }
}

void SampledFunction::init(FuncPtr func, uint32_t sampleCount, float xMin, float xMax){
    AssertExit(!mInitialized, "SampledFunction", "already initialized");
    AssertExit(func != nullptr, "SampledFunction", "null function pointer");

    mSampleCount = sampleCount;
    mHeapAllocated = true;
    m_xMin = xMin;
    m_xMax = xMax;
    m_xRangeInv = 1.0f / (m_xMax - m_xMin);
    mLastIndex = (mSampleCount > 0) ? (mSampleCount - 1) : 0;
    mLastIndexF = static_cast<float>(mLastIndex);

    // allocate and compute samples
    mSamples = new float[mSampleCount];
    const float step = (m_xMax - m_xMin) / static_cast<float>(mLastIndex);
    for(uint32_t i = 0; i < mSampleCount; ++i){
        const float x = m_xMin + step * static_cast<float>(i);
        mSamples[i] = func(x);
    }

    mInitialized = true;
}

void SampledFunction::init(float* samples, bool copy, uint32_t sampleCount, float xMin, float xMax){
    AssertExit(!mInitialized, "SampledFunction", "already initialized");
    AssertExit(samples != nullptr, "SampledFunction", "null sample buffer");

    mSampleCount = sampleCount;
    mHeapAllocated = copy;
    m_xMin = xMin;
    m_xMax = xMax;
    m_xRangeInv = 1.0f / (m_xMax - m_xMin);
    mLastIndex = (mSampleCount > 0) ? (mSampleCount - 1) : 0;
    mLastIndexF = static_cast<float>(mLastIndex);

    // allocate internal buffer and copy samples
    if(copy){
        mSamples = new float[mSampleCount];
        std::memcpy(mSamples, samples, mSampleCount * sizeof(float));
    }
    // use passed buffer (must be kept in memory by user)
    else {
        mSamples = samples;
    }

    mInitialized = true;
}

void SampledFunction::init(std::initializer_list<float> samples,  float xMin, float xMax){
    AssertExit(!mInitialized, "SampledFunction", "already initialized");
    AssertExit(samples.size() > 0, "SampledFunction", "empty initializer list");

    mSampleCount = static_cast<uint32_t>(samples.size());
    mHeapAllocated = true;
    m_xMin = xMin;
    m_xMax = xMax;
    m_xRangeInv = 1.0f / (m_xMax - m_xMin);
    mLastIndex = mSampleCount - 1;
    mLastIndexF = static_cast<float>(mLastIndex);

    // allocate internal buffer and copy from list
    mSamples = new float[mSampleCount];
    uint32_t index = 0;
    for(float value : samples){
        mSamples[index++] = value;
    }

    mInitialized = true;
}

float SampledFunction::evaluate(float x) const {
    if(!mInitialized){
        TriggerDebug("SampledFunction", "evaluate() called before init()");
        return 0.0f;
    }

    if(x <= m_xMin){
        return mSamples[0];
    }
    else if(x >= m_xMax){
        return mSamples[mLastIndex];
    }

    // compute fractional position in table
    const float scaled = (x - m_xMin) * m_xRangeInv * mLastIndexF;
    const uint32_t index = static_cast<uint32_t>(scaled);
    const float frac = scaled - static_cast<float>(index);

    // in case of float rounding error
    if(index >= mLastIndex){
        return mSamples[mLastIndex];
    }

    // linear interpolation
    const float y0 = mSamples[index];
    const float y1 = mSamples[index + 1];
    return y0 + (y1 - y0) * frac;
}

} // namespace Garbox
