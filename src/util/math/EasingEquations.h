#pragma once
#include <math.h>

namespace Garbox {

class EasingEquations {
public:

    // Simple easings
    static float Linear(float t);
    static float InQuad(float t);
    static float OutQuad(float t);
    static float InOutQuad(float t);
    static float InSine(float t);
    static float OutSine(float t);
    static float InOutSine(float t);

    // Elastic easings
    static float InElastic(float t, float amplitude, float period);
    static float OutElastic(float t, float amplitude, float period);
    static float InOutElastic(float t, float amplitude, float period);

};

} // namespace
