#pragma once

#include "util/transform/filter/FilterAbs.h"
#include "assert/Assert.h"
#include <cstdint>
#include <initializer_list>
#include <cstdlib>
#include <cmath>

namespace Garbox {

/**
 * @brief CompositeFilter
 *
 * A stateful composite filter that chains multiple TransformIfc instances.
 *
 * Each transform is applied in order:
 *   y = fN(...f2(f1(x))...)
 *
 * This class itself extends FilterAbs, so it can be used in higher-level filters
 * or as part of other composite filter chains.
 *
 * Supports two construction modes:
 * - External transform pointer array (no heap usage)
 * - Heap-based initializer list (single allocation at startup)
 */
class CompositeFilter : public FilterAbs {
public:
    // external constructor (no heap)
    CompositeFilter(TransformIfc** transforms, uint32_t count);

    // heap-based constructor
    CompositeFilter(std::initializer_list<TransformIfc*> transforms);

    ~CompositeFilter();

    // configuration
    void setInputThreshold(float epsilon);
    float getInputThreshold() const;

protected:
    float onProcess(float value) override;

private:
    TransformIfc** mTransforms = nullptr;
    uint32_t mCount = 0;
    float mInputThreshold = 1e-4f;

    bool mHeapAllocated = false;
};

} // namespace Garbox
