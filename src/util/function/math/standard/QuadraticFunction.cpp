#include "QuadraticFunction.h"

#include "assert/Assert.h"

namespace Garbox {

QuadraticFunction::QuadraticFunction(float a, float b, float c):
    // initialize members
    m_a(a),
    m_b(b),
    m_c(c){
    // nothing to do
}

void QuadraticFunction::setCoefficients(float a, float b, float c){
    m_a = a;
    m_b = b;
    m_c = c;
}

void QuadraticFunction::setPoints(const Point& p1, const Point& p2, const Point& p3){
    setPoints(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void QuadraticFunction::setPoints(float x1, float y1, float x2, float y2, float x3, float y3){
    // ensure unique x-values
    AssertExit(((x1 != x2) && (x1 != x3) && (x2 != x3)), "QuadraticFunction", "duplicate x values in calibration points");

    // solve system of equations for a, b, c
    const float denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
    AssertExit((denom != 0.0f), "QuadraticFunction", "invalid denominator (points too close)");

    m_a = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;
    m_b = (x3*x3 * (y1 - y2) + x2*x2 * (y3 - y1) + x1*x1 * (y2 - y3)) / denom;
    m_c = (x2*x3 * (x2 - x3) * y1 + x3*x1 * (x3 - x1) * y2 + x1*x2 * (x1 - x2) * y3) / denom;
}

float QuadraticFunction::evaluate(float x) const {
    return (m_a * x * x) + (m_b * x) + m_c;
}

float QuadraticFunction::getA() const {
    return m_a;
}

float QuadraticFunction::getB() const {
    return m_b;
}

float QuadraticFunction::getC() const {
    return m_c;
}

} // namespace Garbox
