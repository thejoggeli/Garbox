#pragma once

#include <cstdint>
#include <initializer_list>
#include "FunctionIfc.h"

namespace Garbox {

/**
 * Assumes even sampling of x  
 */
class SampledFunction : public FunctionIfc {
public:
    using FuncPtr = float (*)(float x);

    SampledFunction();
    ~SampledFunction();

    // initialize from function pointer (samples are generated)
    void init(FuncPtr func, uint32_t sampleCount, float xMin, float xMax);

    // initialize from external sample buffer
    void init(float* samples, bool copy, uint32_t sampleCount, float xMin, float xMax);

    // initialize from initializer list (copied into internal buffer)
    void init(std::initializer_list<float> samples, float xMin, float xMax);

    // evaluate sampled function with linear interpolation
    float evaluate(float x) const final;

private:
    float* mSamples = nullptr;
    uint32_t mSampleCount = 0;
    uint32_t mLastIndex = 0;
    float mLastIndexF = 0.0f;
    float m_xMin = 0.0f;
    float m_xMax = 1.0f;
    float m_xRangeInv = 1.0f;
    bool mHeapAllocated = false;
    bool mInitialized = false;
};

} // namespace Garbox
