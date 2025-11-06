#include "AnimatedLed.h"

#include <algorithm>
#include <cmath>
#include "DimmingLed.h"
#include "assert/Assert.h"
#include "core/hardware/ledc/LedcChannel.h"
#include "global/function/FunctionInstances.h"

namespace Garbox {

AnimatedLed::AnimatedLed(LedcChannel& ledChannel) : mLed(ledChannel){
    // nothing to do
}

void AnimatedLed::init(){
    AssertExit(!mInitialized, "AnimatedLed", "already initialized");
    
    // set default smoothing function
    if(mDefaultFunction == nullptr){
        setDefaultFunction(FunctionInstances::GetEaseInSineSampled());
    }

    // init underlying dimming led
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

    const float current = mLed.getBrightness();
    const float target = std::clamp(brightness, 0.0f, 1.0f);
    const float delta = std::fabs(target - current);

    if(delta <= 0.0001f){
        return;
    }

    mActiveFunction = fn;
    mStartBrightness = current;
    mTargetBrightness = target;
    mTimer.start(durationMicros);

    enterState(State::Smoothing);
}

void AnimatedLed::setPlayback(const FunctionIfc& fn, uint32_t cycles, uint32_t periodMicros, float minBrightness, float maxBrightness){
    if(!mInitialized){
        TriggerDebug("AnimatedLed", "not initialized");
        return;
    }

    mActiveFunction = &fn;
    mRemainingCycles = cycles;
    mMinBrightness = std::clamp(minBrightness, 0.0f, 1.0f);
    mMaxBrightness = std::clamp(maxBrightness, 0.0f, 1.0f);

    mTimer.start(periodMicros);
    enterState(State::Playback);
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
        case State::Smoothing:
            handleSmoothingState();
            break;
        case State::Playback:
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
        mActiveFunction = nullptr;
        mTimer.reset();
    }
}

void AnimatedLed::handleSmoothingState(){
    if((mActiveFunction == nullptr) || !mTimer.isRunning()){
        stop();
        return;
    }

    const float t = std::clamp(mTimer.getElapsedFraction(), 0.0f, 1.0f);
    const float eased = mActiveFunction->evaluate(t);
    const float brightness = mStartBrightness + (mTargetBrightness - mStartBrightness) * eased;

    mLed.setBrightness(brightness);

    if(mTimer.isExpired()){
        mLed.setBrightness(mTargetBrightness);
        stop();
    }
}

void AnimatedLed::handlePlaybackState(){
    if(mActiveFunction == nullptr){
        stop();
        return;
    }

    const float t = std::clamp(mTimer.getElapsedFraction(), 0.0f, 1.0f);
    const float y = mActiveFunction->evaluate(t);
    const float brightness = mMinBrightness + (mMaxBrightness - mMinBrightness) * std::clamp(y, 0.0f, 1.0f);

    mLed.setBrightness(brightness);

    if(mTimer.isExpired()){
        if(mRemainingCycles == 0){
            // infinite repeat
            mTimer.restart();
        }
        else if(--mRemainingCycles > 0){
            mTimer.restart();
        }
        else {
            stop();
        }
    }
}

float AnimatedLed::getBrightness() const {
    return mLed.getBrightness();
}

AnimatedLed::State AnimatedLed::getState() const {
    return mState;
}

} // namespace Garbox
