#pragma once

#include <cstdint>

namespace Garbox {

class EasingFunction;
class LedcChannel;

class DimmingLed {
public:

    enum class State : uint8_t {
        Steady = 0,
        Transition,
        Count
    };

    DimmingLed(LedcChannel& ledcChannel);

    void transitionTo(float brightness);
    void setEasingFunction(const EasingFunction& easingFunction);

private:

    LedcChannel& mLedcChannel;
    const EasingFunction* mEasingFunction;

};

} // namespace