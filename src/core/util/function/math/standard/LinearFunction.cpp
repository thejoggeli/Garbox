#include "LinearFunction.h"

#include "core/assert/Assert.h"
#include <cmath>

namespace Garbox {

LinearFunction::LinearFunction(float a, float b):
    // init members
    m_a(a),
    m_b(b){
    // nothing to do
}

void LinearFunction::setCoefficients(float a, float b){
    const bool valid = std::isfinite(a) && std::isfinite(b);
    AssertExit(valid, "LinearFunction", "coefficients must be finite");
    if(!valid){
        return;
    }
    m_a = a;
    m_b = b;
}

void LinearFunction::setPoints(const Point& p1, const Point& p2){
    setPoints(p1.x, p1.y, p2.x, p2.y);
}

void LinearFunction::setPoints(float x1, float y1, float x2, float y2){
    const bool valid =
        std::isfinite(x1) && std::isfinite(y1) &&
        std::isfinite(x2) && std::isfinite(y2) && x1 != x2;
    AssertExit(valid, "LinearFunction", "invalid calibration points");
    if(!valid){
        return;
    }
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
