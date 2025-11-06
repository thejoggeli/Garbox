#include "EaseInOutQuad.h"

namespace Garbox {

float EaseInOutQuad::evaluate(float t) const {
    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;

    if (t < 0.5f)
        return 2.0f * t * t;
    else
        return -1.0f + (4.0f - 2.0f * t) * t;
}

}
