#pragma once

#include "util/transform/filter/composite/CompositeFilter.h"
#include "util/transform/filter/base/SnapFilter.h"
#include "util/transform/filter/base/MovingAverageFilter.h"
#include "util/transform/function/math/ScaleFunction.h"

namespace Garbox {

/**
 * @brief TachoConditioner
 *
 * Signal-conditioning pipeline for RPM or frequency-derived sensor inputs.
 *
 * Pipeline:
 *     LinearFunction -> MovingAverageFilter -> SnapFilter
 *
 * Provides scaling, temporal smoothing, and quantization stability
 * typical for rotational speed or frequency measurements.
 */
class TachoConditioner {
public:
    TachoConditioner(uint32_t windowSize, float fixedPointScaleFactor = 1000.0f);
    ~TachoConditioner() = default;

    // reset
    void reset();

    // configuration
    void setInputThreshold(float threshold);
    void setInputScaling(float scaling);
    void setFixedPointScaling(float scaling);
    void setOutputSnapping(float resolution, float stickiness);

    // data processing
    float process(float value);

    // getters
    float getFilteredValue() const;
    float getRawValue() const;

private:
    static constexpr size_t NumFilters = 3;

    ScaleFunction mScaleFunction;
    MovingAverageFilter mMovingAverageFilter;
    SnapFilter mSnapFilter;

    TransformIfc* mChain[NumFilters];
    CompositeFilter mCompositeFilter;
};

} // namespace Garbox
