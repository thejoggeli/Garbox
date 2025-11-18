#pragma once

#include "core/util/function/filter/FilterFunctionAbs.h"
#include <cmath>

namespace Garbox {

/**
 * @brief SnapFilter
 *
 * Quantizes the input value to discrete steps defined by a resolution.
 * Includes optional "stickiness" to prevent rapid oscillation around boundaries.
 *
 * Example:
 *   resolution = 0.1
 *   stickiness = 0.5  (value must move half a step beyond the snap threshold)
 *
 * Result: Values between 1.05 and 1.15 all snap to 1.1
 *         Once snapped to 1.1 the value must <1.025 or >1.175 to exit the "sticky zone" 
 */
class SnapFilter : public FilterFunctionAbs {
public:
    SnapFilter();
    SnapFilter(float resolution, float stickiness = 0.5f);

    void setResolution(float resolution);
    void setStickiness(float stickiness);

    float getResolution() const;
    float getStickiness() const;

protected:
    float onProcess(float value) override;

private:
    float mResolution = 0.1f;     // quantization step (e.g. 0.1)
    float mStickiness = 0.5f;     // fraction of resolution required to trigger snap
    float mHalfStickRange = 0.0f; // precomputed 0.5 * stickiness * resolution

    void updateStickRange();
};

} // namespace Garbox
