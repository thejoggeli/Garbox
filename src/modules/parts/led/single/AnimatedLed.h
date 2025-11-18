#pragma once

#include <cstdint>
#include "DimmingLed.h"
#include "core/time/SoftwareTimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace Garbox {

class MathFunctionIfc;
class LedcChannel;

class AnimatedLed {
public:

    enum class State : uint8_t {
        Static,
        Animating
    };

    AnimatedLed(LedcChannel& ledcChannel);

    virtual void init();

    virtual void setDefaultFunction(const MathFunctionIfc& fn);
    virtual void setBrightness(float brightness);
    virtual void setBrightnessSmooth(float brightness, uint32_t durationMicros, const MathFunctionIfc* fn = nullptr);
    virtual void setAnimation(const MathFunctionIfc& fn, uint32_t cycles, uint32_t durationMicros, float yStart=0.0f, float yEnd=1.0f);

    virtual void animationAddFrame(const MathFunctionIfc& fn, uint32_t durationMicros, float yStart=0.0f, float yEnd=1.0f);
    virtual void animationAddDelay(uint32_t durationMicros);
    virtual void animationStart(uint32_t cycles = 1);
    virtual void animationClear();

    virtual void stop();
    virtual void tick();

    float getBrightness() const;
    State getState() const;
    bool isInitialized() const;
    bool isAnimationPlaying() const;

    // Disallow copy and move 
    AnimatedLed(const AnimatedLed&) = delete;
    AnimatedLed& operator=(const AnimatedLed&) = delete;
    AnimatedLed(AnimatedLed&&) = delete;
    AnimatedLed& operator=(AnimatedLed&&) = delete;

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

    SemaphoreHandle_t mMutex = nullptr;

    void enterState(State state);
    void handlePlaybackState();
};

} // namespace Garbox
