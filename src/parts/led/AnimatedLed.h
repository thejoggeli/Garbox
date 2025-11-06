#pragma once

#include <cstdint>
#include "DimmingLed.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class FunctionIfc;
class LedcChannel;

class AnimatedLed {
public:

    enum class State : uint8_t {
        Static,
        Smoothing,
        Playback
    };

    explicit AnimatedLed(LedcChannel& ledcChannel);

    void init();

    void setDefaultFunction(const FunctionIfc& fn);

    void setBrightness(float brightness);
    void setBrightnessSmooth(float brightness, uint32_t durationMicros, const FunctionIfc* fn = nullptr);
    void setPlayback(const FunctionIfc& fn, uint32_t cycles = 0, uint32_t periodMicros = 1'000'000, float minBrightness = 0.0f, float maxBrightness = 1.0f);

    void stop();
    void tick();

    float getBrightness() const;
    State getState() const;

private:
    DimmingLed mLed;

    const FunctionIfc* mDefaultFunction = nullptr;
    const FunctionIfc* mActiveFunction = nullptr;

    SoftwareTimer mTimer;

    State mState = State::Static;
    bool mInitialized = false;

    float mStartBrightness = 0.0f;
    float mTargetBrightness = 0.0f;
    float mMinBrightness = 0.0f;
    float mMaxBrightness = 1.0f;

    uint32_t mRemainingCycles = 0; // 0 = infinite

    void enterState(State state);
    void handleSmoothingState();
    void handlePlaybackState();

};

} // namespace Garbox
