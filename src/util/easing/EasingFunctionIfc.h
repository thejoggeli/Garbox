#pragma once

// Common interface for all easing functions
class EasingFunctionIfc {
public:
    virtual ~EasingFunctionIfc() = default;

    // Compute eased value for normalized time t in [0, 1]
    virtual float evaluate(float t) const = 0;
};
