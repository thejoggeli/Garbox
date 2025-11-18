#pragma once

#include "core/util/function/math/MathFunctionIfc.h"

namespace Garbox {

/**
 * @brief LinearFunction
 *
 * Stateless function implementing a linear relationship:
 *     y = a * x + b
 *
 * Can be configured using two calibration points (x1,y1,x2,y2)
 * or direct coefficients (a,b).
 */
class LinearFunction : public MathFunctionIfc {
public:

    struct Point {
        float x;
        float y;
    };

    LinearFunction(float a = 1.0f, float b = 0.0f);
    ~LinearFunction() = default;

    // configure using slope/intercept directly
    void setCoefficients(float a, float b);

    // configure using two calibration points
    void setPoints(const Point& p1, const Point& p2);
    void setPoints(float x1, float y1, float x2, float y2);

    // evaluate y = a*x + b
    float evaluate(float x) const final;

    float getSlope() const;
    float getOffset() const;

private:
    float m_a = 1.0f;      // slope
    float m_b = 0.0f;      // offset
};

} // namespace Garbox
