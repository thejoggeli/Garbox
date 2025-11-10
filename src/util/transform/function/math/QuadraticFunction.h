#pragma once

#include "util/transform/function/FunctionIfc.h"

namespace Garbox {

/**
 * @brief QuadraticFunction
 *
 * Implements a quadratic mapping:
 *     y = a*x² + b*x + c
 *
 * Can be configured either via direct coefficients (a,b,c)
 * or by providing three calibration points.
 */
class QuadraticFunction : public FunctionIfc {
public:

    struct Point {
        float x;
        float y;
    };

    QuadraticFunction(float a = 1.0f, float b = 0.0f, float c = 0.0f);
    ~QuadraticFunction() = default;

    // configure using direct coefficients
    void setCoefficients(float a, float b, float c);

    // configure using three calibration points
    void setPoints(const Point& p1, const Point& p2, const Point& p3);
    void setPoints(
        float x1, float y1,
        float x2, float y2,
        float x3, float y3
    );

    // evaluate y = a*x² + b*x + c
    float evaluate(float x) const final;

    float getA() const;
    float getB() const;
    float getC() const;

private:
    float m_a;
    float m_b;
    float m_c;
};

} // namespace Garbox
