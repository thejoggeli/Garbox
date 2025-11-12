#pragma once

#include <cstdint>
#include "DimmingLed.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class MathFunctionIfc;
class LedcChannel;

class AnimatedLed {
public:

    enum class State : uint8_t {
        Static,
        Animating
    };

    explicit AnimatedLed(LedcChannel& ledcChannel);

    void init();

    void setDefaultFunction(const MathFunctionIfc& fn);

    void setBrightness(float brightness);
    void setBrightnessSmooth(float brightness, uint32_t durationMicros, const MathFunctionIfc* fn = nullptr);
    void setAnimation(const MathFunctionIfc& fn, uint32_t cycles, uint32_t durationMicros, float yStart=0.0f, float yEnd=1.0f);

    void animationAddFrame(const MathFunctionIfc& fn, uint32_t durationMicros, float yStart=0.0f, float yEnd=1.0f);
    void animationAddDelay(uint32_t durationMicros);
    void animationStart(uint32_t cycles = 1);
    void animationClear();

    void stop();
    void tick();

    float getBrightness() const;
    State getState() const;
    bool isInitialized() const;
    bool isAnimationPlaying() const;

private:

    struct PlaybackFrame {
        const MathFunctionIfc* function = nullptr;
        uint32_t durationMicros = 0;
        float yStart = 0.0f;
        float yEnd = 0.0f;
    };

    static constexpr uint8_t MaxPlaybackFrames = 16;

    DimmingLed mLed;

    const MathFunctionIfc* mDefaultFunction = nullptr;

    SoftwareTimer mTimer;

    State mState = State::Static;
    bool mInitialized = false;

    uint32_t mRemainingCycles = 0; // 0 = infinite

    PlaybackFrame mFrames[MaxPlaybackFrames];
    uint8_t mFrameCount = 0;
    uint8_t mCurrentFrame = 0;

    void enterState(State state);
    void handlePlaybackState();
};

} // namespace Garbox
