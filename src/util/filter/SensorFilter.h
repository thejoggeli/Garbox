#pragma once

#include "ExponentialFilter.h"
#include <cmath>
#include <cstdint>

namespace Garbox {

/**
 * @brief SensorFilter
 *
 * Combines linear calibration, exponential filtering, and optional value snapping.
 * Typical use: smoothing ADC readings and converting raw sensor values to physical units.
 */
class SensorFilter {
public:

    struct Point {
        float x;
        float y;
    };

    SensorFilter();

    // Configure the calibration and filter
    void setCalibrationPoints(const Point& p1, const Point& p2);
    void setCalibrationPoints(float x1, float y1, float x2, float y2);
    void setAlpha(float alpha);
    void setAlphaComputed(float fraction, uint32_t ticks);
    void setThreshold(float threshold);
    void setSnapResolution(float snapResolution);

    // Update and get current sensor reading
    void update(float rawValue);
    float getValue() const;

private:
    float applyCalibration(float rawValue) const;
    float applySnap(float value) const;

    ExponentialFilter mFilter;

    float mSlope = 1.0f;
    float mOffset = 0.0f;
    float mSnapResolution = 0.0f; // 0 = snapping disabled
    bool mCalibrated = false;
};

} // namespace Garbox
