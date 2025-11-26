#pragma once

#include <cstdint>

namespace Garbox {

class PidControl {
public:
    PidControl(
        float Kp,
        float Ki,
        float Kd,
        float minOutput,
        float maxOutput
    );

    void init();

    float step(
        float measured,
        float target,
        float dtSeconds
    );

    void reset();

private:

    // gains
    float mKp;
    float mKi;
    float mKd;

    // output limits
    float mMinOutput;
    float mMaxOutput;

    // state
    float mIntegral = 0.0f;
    float mPrevError = 0.0f;
    float mOutput = 0.0f;
    bool mFirst = true;
};

} // namespace Garbox
