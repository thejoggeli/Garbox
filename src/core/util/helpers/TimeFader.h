#pragma once

#include <cstdint>

namespace Garbox {

class MathFunctionIfc;

class TimeFader {
public:

    TimeFader();
    TimeFader(const MathFunctionIfc& function);

    void start(float targetValue, uint32_t durationMicros);
    void start(float startValue, float targetValue, uint32_t durationMicros);

    void setEasingFunction(const MathFunctionIfc& function);
    void setClampEnabled(bool enabled);

    float updateValue();

    bool isActive() const;
    bool isFinished() const;
    float getCurrentValue() const;
    float getTargetValue() const;

private:
    bool mInitialized = false;

    float mStartValue = 0.0f;
    float mCurrentValue = 0.0f;
    float mTargetValue = 0.0f;
    bool mClampEnabled = true;

    uint32_t mStartTimeMicros = 0;
    uint32_t mDurationMicros = 0;

    const MathFunctionIfc* mEasingFunction = nullptr;

    bool mActive = false;
    bool mFinished = false;
};

} // namespace
