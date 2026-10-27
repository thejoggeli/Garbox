#include "SampledFunction.h"

#include <cmath>
#include <cstring>
#include <limits>
#include <new>
#include "core/assert/Assert.h"

namespace Garbox {

namespace {

bool validDomain(uint32_t sampleCount, float xMin, float xMax){
    return sampleCount >= 2 && std::isfinite(xMin) && std::isfinite(xMax) && xMax > xMin;
}

bool validSamples(const float* samples, uint32_t sampleCount){
    if(samples == nullptr){
        return false;
    }
    for(uint32_t index = 0; index < sampleCount; ++index){
        if(!std::isfinite(samples[index])){
            return false;
        }
    }
    return true;
}

} // namespace

SampledFunction::SampledFunction(){
    // nothing to do
}

SampledFunction::~SampledFunction(){
    if(mHeapAllocated){
        delete[] mSamples;
    }
}

void SampledFunction::init(FuncPtr func, uint32_t sampleCount, float xMin, float xMax){
    AssertExit(!mInitialized, "SampledFunction", "already initialized");
    if(mInitialized){
        return;
    }
    AssertExit(func != nullptr, "SampledFunction", "null function pointer");
    AssertExit(validDomain(sampleCount, xMin, xMax), "SampledFunction", "invalid sample domain");
    if(func == nullptr || !validDomain(sampleCount, xMin, xMax)){
        return;
    }

    mSampleCount = sampleCount;
    mHeapAllocated = true;
    m_xMin = xMin;
    m_xMax = xMax;
    m_xRangeInv = 1.0f / (m_xMax - m_xMin);
    mLastIndex = (mSampleCount > 0) ? (mSampleCount - 1) : 0;
    mLastIndexF = static_cast<float>(mLastIndex);

    // allocate and compute samples
    float* ownedSamples = new (std::nothrow) float[mSampleCount];
    AssertExit(ownedSamples != nullptr, "SampledFunction", "sample allocation failed");
    if(ownedSamples == nullptr){
        mHeapAllocated = false;
        return;
    }
    mSamples = ownedSamples;
    const float step = (m_xMax - m_xMin) / static_cast<float>(mLastIndex);
    for(uint32_t i = 0; i < mSampleCount; ++i){
        const float x = m_xMin + step * static_cast<float>(i);
        ownedSamples[i] = func(x);
        if(!std::isfinite(ownedSamples[i])){
            AssertExit(false, "SampledFunction", "function produced a non-finite sample");
            delete[] ownedSamples;
            mSamples = nullptr;
            mHeapAllocated = false;
            return;
        }
    }

    mInitialized = true;
}

void SampledFunction::init(const float* samples, bool copy, uint32_t sampleCount, float xMin, float xMax){
    AssertExit(!mInitialized, "SampledFunction", "already initialized");
    if(mInitialized){
        return;
    }
    AssertExit(samples != nullptr, "SampledFunction", "null sample buffer");
    AssertExit(validDomain(sampleCount, xMin, xMax), "SampledFunction", "invalid sample domain");
    AssertExit(validSamples(samples, sampleCount), "SampledFunction", "samples must be finite");
    if(!validDomain(sampleCount, xMin, xMax) || !validSamples(samples, sampleCount)){
        return;
    }

    mSampleCount = sampleCount;
    mHeapAllocated = copy;
    m_xMin = xMin;
    m_xMax = xMax;
    m_xRangeInv = 1.0f / (m_xMax - m_xMin);
    mLastIndex = (mSampleCount > 0) ? (mSampleCount - 1) : 0;
    mLastIndexF = static_cast<float>(mLastIndex);

    // allocate internal buffer and copy samples
    if(copy){
        float* ownedSamples = new (std::nothrow) float[mSampleCount];
        AssertExit(ownedSamples != nullptr, "SampledFunction", "sample allocation failed");
        if(ownedSamples == nullptr){
            mHeapAllocated = false;
            return;
        }
        std::memcpy(ownedSamples, samples, mSampleCount * sizeof(float));
        mSamples = ownedSamples;
    }
    // use passed buffer (must be kept in memory by user)
    else {
        mSamples = samples;
    }

    mInitialized = true;
}

void SampledFunction::init(std::initializer_list<float> samples,  float xMin, float xMax){
    AssertExit(!mInitialized, "SampledFunction", "already initialized");
    if(mInitialized){
        return;
    }
    const bool sizeFits = samples.size() <= std::numeric_limits<uint32_t>::max();
    AssertExit(sizeFits, "SampledFunction", "too many samples");
    const uint32_t sampleCount = sizeFits ? static_cast<uint32_t>(samples.size()) : 0;
    AssertExit(validDomain(sampleCount, xMin, xMax), "SampledFunction", "invalid sample domain");
    AssertExit(validSamples(samples.begin(), sampleCount), "SampledFunction", "samples must be finite");
    if(!sizeFits || !validDomain(sampleCount, xMin, xMax) ||
       !validSamples(samples.begin(), sampleCount)){
        return;
    }

    mSampleCount = sampleCount;
    mHeapAllocated = true;
    m_xMin = xMin;
    m_xMax = xMax;
    m_xRangeInv = 1.0f / (m_xMax - m_xMin);
    mLastIndex = mSampleCount - 1;
    mLastIndexF = static_cast<float>(mLastIndex);

    // allocate internal buffer and copy from list
    float* ownedSamples = new (std::nothrow) float[mSampleCount];
    AssertExit(ownedSamples != nullptr, "SampledFunction", "sample allocation failed");
    if(ownedSamples == nullptr){
        mHeapAllocated = false;
        return;
    }
    mSamples = ownedSamples;
    uint32_t index = 0;
    for(float value : samples){
        ownedSamples[index++] = value;
    }

    mInitialized = true;
}

float SampledFunction::evaluate(float x) const {
    if(!mInitialized){
        TriggerDebug("SampledFunction", "evaluate() called before init()");
        return 0.0f;
    }
    if(!std::isfinite(x)){
        AssertExit(false, "SampledFunction", "input must be finite");
        return mSamples[0];
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
