#pragma once

#include <cstdint>
#include <initializer_list>
#include "core/util/function/math/MathFunctionIfc.h"

namespace Garbox {

/**
 * @brief Sparse lookup table for arbitrary (x, y) mappings.
 *
 * Evaluates using either linear interpolation or zero-order hold between stored points.
 * 
 * X-values must be strictly increasing.
 *
 * Typical use cases include calibration curves, sensor correction maps and piecewise-defined transfer characteristics.
 */
class LookupFunction : public MathFunctionIfc {
public:

    struct Point {
        float x;
        float y;
    };

    enum class Mode : uint8_t {
        Linear, // standard linear interpolation
        Hold    // zero-order hold (step)
    };

    LookupFunction();
    ~LookupFunction();

    void init(Point* points, bool copy, uint32_t pointCount, Mode mode = Mode::Linear);
    void init(std::initializer_list<Point> points, Mode mode = Mode::Linear);

    float evaluate(float x) const;

private:
    Point* mPoints = nullptr;
    uint32_t mPointCount = 0;
    uint32_t mLastIndex = 0;
    bool mHeapAllocated = false;
    bool mInitialized = false;
    Mode mMode = Mode::Linear;
};

} // namespace Garbox
