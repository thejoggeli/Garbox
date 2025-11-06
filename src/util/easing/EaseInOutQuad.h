#pragma once

#include "EasingFunctionIfc.h"

namespace Garbox {

// Quadratic ease-in-out: accelerates, then decelerates
// f(t) = 2*t^2            for t < 0.5
// f(t) = -1 + (4 - 2*t)*t for t >= 0.5
class EaseInOutQuad : public EasingFunctionIfc {
public:
    float evaluate(float t) const final;
};

}