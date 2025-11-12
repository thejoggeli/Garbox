#pragma once

#include "util/function/filter/composite/CompositeFilter.h"
#include "util/function/filter/filters/SnapFilter.h"
#include "util/function/filter/filters/MovingAverageFilter.h"
#include "util/function/math/standard/ScaleFunction.h"

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
    TachoConditioner(uint32_t windowSize, float fixedPointScaling = 1000.0f);
    ~TachoConditioner() = default;

    // reset
    void reset();

    // configuration
    void setInputScaling(float scaling);
    void setFixedPointScaling(float scaling);
    void setOutputSnapping(float resolution, float stickiness);

    // data processing
    float process(float value);

    // getters
    float getFilteredValue() const;
    float getRawValue() const;
    float getUnfilteredValue() const; // get scaled but otherwise unfiltered value

private:
    static constexpr size_t NumFilters = 3;

    ScaleFunction mScaleFunction;
    MovingAverageFilter mMovingAverageFilter;
    SnapFilter mSnapFilter;

    FunctionIfc* mChain[NumFilters];
    float mResults[NumFilters];
    CompositeFilter mCompositeFilter;
};

} // namespace Garbox
