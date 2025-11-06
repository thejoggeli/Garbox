#pragma once
#include <math.h>

namespace Garbox {

class EasingFunctions {
public:

    // Simple easings
    static float EaseLinear(float t);
    static float EaseInQuad(float t);
    static float EaseOutQuad(float t);
    static float EaseInOutQuad(float t);
    static float EaseInSine(float t);
    static float EaseOutSine(float t);
    static float EaseInOutSine(float t);

    // Elastic easings
    static float EaseInElastic(float t, float amplitude, float period);
    static float EaseOutElastic(float t, float amplitude, float period);
    static float EaseInOutElastic(float t, float amplitude, float period);

};

} // namespace
