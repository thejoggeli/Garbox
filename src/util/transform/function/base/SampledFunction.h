#pragma once

#include <cstdint>
#include <initializer_list>
#include "util/transform/function/FunctionIfc.h"

namespace Garbox {

/**
 * @brief Discrete sampled function with uniform x-spacing.
 *
 * Represents a continuous function by a fixed number of evenly spaced samples.
 * Evaluates using linear interpolation between stored samples.
 *
 * Supports three initialization modes:
 *  - From an analytic function (auto-generated samples)
 *  - From an external buffer (copied or referenced)
 *  - From an initializer list
 *
 * Suitable for approximating slow-changing or nonlinear analytic functions 
 * without runtime computation cost.
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
