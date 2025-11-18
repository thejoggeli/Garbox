#pragma once

#include "core/util/function/filter/composite/CompositeFilter.h"
#include "core/util/function/filter/filters/MovingAverageFilter.h"
#include "core/util/function/filter/filters/SnapFilter.h"
#include "core/util/function/math/standard/LinearFunction.h"

namespace Garbox {

/**
 * @brief AdcConditioner
 *
 * Signal-conditioning pipeline for analog sensor inputs.
 * 
 * Pipeline:
 *     LinearFunction -> MovingAverageFilter -> SnapFilter
 *
 * Handles calibration, smoothing, and quantization of ADC values.
 * Designed for low-frequency signals (e.g., voltage or current sensing).
 */
class AdcConditioner {
public:
    AdcConditioner(uint32_t windowSize, float fixedPointScaling = 1000.0);
    ~AdcConditioner() = default;

    // reset
    void reset();

    // configuration
    void setCalibrationPoints(LinearFunction::Point p1, LinearFunction::Point p2);
    void setCalibrationPoints(float x1, float y1, float x2, float y2);
    void setFixedPointScaling(float scaling);
    void setSnapping(float resolution, float stickiness);

    // data processing
    float process(float value);

    // getters
    float getFilteredValue() const;
    float getRawValue() const;
    float getUnfilteredValue() const; // returns calibrated but otherwise unfiltered value

private:

    static constexpr size_t NumFilters = 3;

    LinearFunction mLinearFunction;
    MovingAverageFilter mMovingAverageFilter;
    SnapFilter mSnapFilter;

    FunctionIfc* mChain[NumFilters];
    float mResults[NumFilters];
    CompositeFilter mCompositeFilter;
};

} // namespace Garbox
