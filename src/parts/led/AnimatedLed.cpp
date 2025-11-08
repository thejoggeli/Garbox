#include "AnimatedLed.h"

#include <algorithm>
#include <cmath>
#include "assert/Assert.h"
#include "core/hardware/ledc/LedcChannel.h"
#include "global/function/FunctionInstances.h"

namespace Garbox {

AnimatedLed::AnimatedLed(LedcChannel& ledChannel):
    mLed(ledChannel){
    // nothing to do
}

void AnimatedLed::init(){
    AssertExit(!mInitialized, "AnimatedLed", "already initialized");

    if(mDefaultFunction == nullptr){
        setDefaultFunction(FunctionInstances::GetEaseInSineSampled());
    }

    mLed.init();
    mInitialized = true;
}

void AnimatedLed::setDefaultFunction(const FunctionIfc& fn){
    mDefaultFunction = &fn;
}

void AnimatedLed::setBrightness(float brightness){
    if(!mInitialized){
        TriggerDebug("AnimatedLed", "not initialized");
        return;
    }
    stop();
    mLed.setBrightness(brightness);
}

void AnimatedLed::setBrightnessSmooth(float brightness, uint32_t durationMicros, const FunctionIfc* fn){
    if(!mInitialized){
        TriggerDebug("AnimatedLed", "not initialized");
        return;
    }

    if(fn == nullptr){
        fn = mDefaultFunction;
    }

    if(fn == nullptr){
        TriggerDebug("AnimatedLed", "no function provided or set as default");
        setBrightness(brightness);
        return;
    }

    const float start = mLed.getBrightness();
    const float end = std::clamp(brightness, 0.0f, 1.0f);

    animationClear();
    animationAddFrame(*fn, durationMicros, start, end);
    animationStart(1);
}

void AnimatedLed::setAnimation(const FunctionIfc& fn, uint32_t cycles, uint32_t durationMicros, float yStart, float yEnd){
    if(mFrameCount >= MaxPlaybackFrames){
        TriggerDebug("AnimatedLed", "playback buffer full");
        return;
    }
    animationClear();
    animationAddFrame(fn, durationMicros, yStart, yEnd);
    animationStart(cycles);
}

void AnimatedLed::animationAddFrame(const FunctionIfc& fn, uint32_t durationMicros, float yStart, float yEnd){
    if(mFrameCount >= MaxPlaybackFrames){
        TriggerDebug("AnimatedLed", "playback buffer full");
        return;
    }
    if(durationMicros == 0){
        TriggerDebug("AnimatedLed", "added duration=0 frame");
        return;
    }

    PlaybackFrame& frame = mFrames[mFrameCount++];
    frame.function = &fn;
    frame.yStart = std::clamp(yStart, 0.0f, 1.0f);
    frame.yEnd = std::clamp(yEnd, 0.0f, 1.0f);
    frame.durationMicros = durationMicros;
}

void AnimatedLed::animationAddDelay(uint32_t durationMicros){
    if(mFrameCount >= MaxPlaybackFrames){
        TriggerDebug("AnimatedLed", "playback buffer full");
        return;
    }
    if(durationMicros == 0){
        TriggerDebug("AnimatedLed", "added duration=0 frame");
        return;
    }

    PlaybackFrame& frame = mFrames[mFrameCount++];
    frame.function = nullptr;
    frame.durationMicros = durationMicros;
}

void AnimatedLed::animationStart(uint32_t cycles){
    if(!mInitialized){
        TriggerDebug("AnimatedLed", "not initialized");
        return;
    }

    if(mFrameCount == 0){
        TriggerDebug("AnimatedLed", "no playback frames added");
        return;
    }

    mRemainingCycles = cycles;
    mCurrentFrame = 0;

    // start playback timer
    mTimer.start(mFrames[mCurrentFrame].durationMicros);

    enterState(State::Animating);
}

void AnimatedLed::animationClear(){
    mFrameCount = 0;
    mCurrentFrame = 0;
    stop();
}

void AnimatedLed::stop(){
    if(mState == State::Static){
        return;
    }
    enterState(State::Static);
}

void AnimatedLed::tick(){
    if(!mInitialized){
        TriggerDebug("AnimatedLed", "not initialized");
        return;
    }

    switch(mState){
        case State::Static:
            break;
        case State::Animating:
            handlePlaybackState();
            break;
        default:
            TriggerDebug("AnimatedLed", "tick() unhandled state");
            break;
    }
}

void AnimatedLed::enterState(State state){
    mState = state;
    if(state == State::Static){
        mCurrentFrame = 0;
        mTimer.reset();
    }
}

void AnimatedLed::handlePlaybackState(){
    if(mFrameCount == 0){
        stop();
        return;
    }

    PlaybackFrame& frame = mFrames[mCurrentFrame];

    if(frame.function != nullptr){
        const float x = mTimer.getElapsedFraction(true);
        const float y = frame.function->evaluate(x);
        const float brightness = frame.yStart + (frame.yEnd - frame.yStart) * y;
        mLed.setBrightness(brightness);
    }

    // handle frame finished
    const bool frameFinished = mTimer.isExpired();
    if(frameFinished){
        mCurrentFrame++;

        // handle cycle finished
        const bool cycleFinished = mCurrentFrame >= mFrameCount; 
        if(cycleFinished){

            // check if animation finished
            const bool isInfinite = (mRemainingCycles == 0);
            const bool lastCycle = (mRemainingCycles == 1);
            if(lastCycle && !isInfinite){
                stop();
                return;
            }

            // start next cycle
            mCurrentFrame = 0;
            mRemainingCycles--;
        }

        // restart timer for next frame
        mTimer.restart(mFrames[mCurrentFrame].durationMicros);
    }
}

float AnimatedLed::getBrightness() const {
    return mLed.getBrightness();
}

AnimatedLed::State AnimatedLed::getState() const {
    return mState;
}

bool AnimatedLed::isInitialized() const { 
    return mInitialized; 
}

} // namespace Garbox
