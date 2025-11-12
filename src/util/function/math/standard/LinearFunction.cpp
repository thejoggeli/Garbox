#include "LinearFunction.h"

#include "assert/Assert.h"

namespace Garbox {

LinearFunction::LinearFunction(float a, float b):
    // init members
    m_a(a),
    m_b(b){
    // nothing to do
}

void LinearFunction::setCoefficients(float a, float b){
    m_a = a;
    m_b = b;
}

void LinearFunction::setPoints(const Point& p1, const Point& p2){
    setPoints(p1.x, p1.y, p2.x, p2.y);
}

void LinearFunction::setPoints(float x1, float y1, float x2, float y2){
    AssertExit((x1 != x2), "LinearFunction", "invalid calibration points (identical x)");
    m_a = (y2 - y1) / (x2 - x1);
    m_b = y1 - (m_a * x1);
}

float LinearFunction::evaluate(float x) const {
    return (m_a * x) + m_b;
}

float LinearFunction::getSlope() const {
    return m_a;
}

float LinearFunction::getOffset() const {
    return m_b;
}

} // namespace Garbox
