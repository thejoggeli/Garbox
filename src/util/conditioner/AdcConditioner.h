#pragma once

#include "util/transform/filter/composite/CompositeFilter.h"
#include "util/transform/filter/base/ExponentialFilter.h"
#include "util/transform/filter/base/SnapFilter.h"
#include "util/transform/function/math/LinearFunction.h"

namespace Garbox {

/**
 * @brief AdcConditioner
 *
 * Signal-conditioning pipeline for analog sensor inputs.
 * 
 * Pipeline:
 *     LinearFunction -> ExponentialFilter -> SnapFilter
 *
 * Handles calibration, smoothing, and quantization of ADC values.
 * Designed for low-frequency signals (e.g., voltage or current sensing).
 */
class AdcConditioner {
public:
    AdcConditioner();
    ~AdcConditioner() = default;

    // reset
    void reset();

    // configuration
    void setInputThreshold(float threshold);
    void setCalibrationPoints(LinearFunction::Point p1, LinearFunction::Point p2);
    void setCalibrationPoints(float x1, float y1, float x2, float y2);
    void setAlpha(float alpha);
    void setAlphaComputed(float fraction, uint32_t ticks);
    void setSnapping(float resolution, float stickiness);

    // data processing
    float process(float value);

    // getters
    float getFilteredValue() const;
    float getRawValue() const;

private:

    static constexpr size_t NumFilters = 3;

    LinearFunction mLinearFunction;
    ExponentialFilter mExponentialFilter;
    SnapFilter mSnapFilter;

    TransformIfc* mChain[NumFilters];
    CompositeFilter mCompositeFilter;
};

} // namespace Garbox
