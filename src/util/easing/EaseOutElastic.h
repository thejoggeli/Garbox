#pragma once

#include "EasingFunctionIfc.h"

// Elastic ease-out: overshoots with decreasing oscillation
// Formula uses amplitude (A) and period (P) to control the shape
class EaseOutElastic : public EasingFunctionIfc {
public:
    EaseOutElastic(float amplitude = 1.0f, float period = 0.3f);

    // Evaluate eased value for normalized time t in [0,1]
    float evaluate(float t) const final;

    // Optional runtime configuration
    void setAmplitude(float amplitude);
    void setPeriod(float period);

private:
    float mAmplitude;
    float mPeriod;
};
