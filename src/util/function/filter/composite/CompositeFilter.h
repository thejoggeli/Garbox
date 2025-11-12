#pragma once

#include "util/function/filter/FilterFunctionAbs.h"
#include "assert/Assert.h"
#include <cstdint>
#include <initializer_list>
#include <cstdlib>
#include <cmath>

namespace Garbox {

/**
 * @brief CompositeFilter
 *
 * A stateful composite filter that chains multiple FunctionIfc instances.
 *
 * Each transform is applied in order:
 *   y = fN(...f2(f1(x))...)
 *
 * This class itself extends FilterFunctionAbs, so it can be used in higher-level filters
 * or as part of other composite filter chains.
 *
 * Supports two construction modes:
 * - External transform pointer array (no heap usage)
 * - Heap-based initializer list (single allocation at startup)
 */
class CompositeFilter : public FilterFunctionAbs {
public:
    // external constructor (no heap)
    CompositeFilter(FunctionIfc** transforms, float* results, uint32_t count);

    // heap-based constructor
    CompositeFilter(std::initializer_list<FunctionIfc*> transforms, bool storeResults = true);

    ~CompositeFilter();

    // configuration
    void setInputThreshold(float epsilon);
    float getInputThreshold() const;
    float getResult(size_t index) const;

protected:
    float onProcess(float value) override;

private:
    FunctionIfc** mTransforms = nullptr;
    float* mResults = nullptr;
    uint32_t mCount = 0;
    float mInputThreshold = 0.0f;

    bool mHeapAllocated = false;
};

} // namespace Garbox
