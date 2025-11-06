#include "SmoothLed.h"

#include <algorithm>
#include <cmath>
#include "DimmingLed.h"
#include "assert/Assert.h"
#include "global/function/FunctionInstances.h"

namespace Garbox {

SmoothLed::SmoothLed(DimmingLed& led) : mLed(led){
    // nothing to do
}

SmoothLed::~SmoothLed(){
    TriggerExit("SmoothLed", "heap using classes must not be deconstructed");
}

void SmoothLed::init(){
    AssertExit(!mInitialized, "SmoothLed", "already initialized");
    
    // set default smoothing function
    if(mDefaultFunction == nullptr){
        setDefaultFunction(FunctionInstances::GetEaseInSineSampled());
    }

    mInitialized = true;
}

void SmoothLed::setDefaultFunction(const FunctionIfc& fn){
    mDefaultFunction = &fn;
}

void SmoothLed::setBrightness(float brightness){
    if(!mInitialized){
        TriggerDebug("SmoothLed", "not initialized");
        return;
    }
    stop();
    mLed.setBrightness(brightness);
}

void SmoothLed::setBrightnessSmooth(float brightness, float speedBrightnessPerSec, const FunctionIfc* fn){
    if(!mInitialized){
        TriggerDebug("SmoothLed", "not initialized");
        return;
    }

    if(fn == nullptr){
        fn = mDefaultFunction;
    }

    if(fn == nullptr){
        TriggerDebug("SmoothLed", "no function provided or set as default");
        setBrightness(brightness);
        return;
    }

    const float current = mLed.getBrightness();
    const float target = std::clamp(brightness, 0.0f, 1.0f);
    const float delta = std::fabs(target - current);

    if(delta <= 0.0001f){
        return;
    }

    const float durationSeconds = delta / speedBrightnessPerSec;
    const uint32_t durationMicros = static_cast<uint32_t>(durationSeconds * 1e6f);

    mActiveFunction = fn;
    mStartBrightness = current;
    mTargetBrightness = target;
    mTimer.start(durationMicros);

    enterState(State::Smoothing);
}

void SmoothLed::setPlayback(const FunctionIfc& fn, uint32_t cycles, float speedHz, float minBrightness, float maxBrightness){
    if(!mInitialized){
        TriggerDebug("SmoothLed", "not initialized");
        return;
    }

    speedHz = (speedHz <= 0.0f) ? 1.0f : speedHz;

    mActiveFunction = &fn;
    mRemainingCycles = cycles;
    mMinBrightness = std::clamp(minBrightness, 0.0f, 1.0f);
    mMaxBrightness = std::clamp(maxBrightness, 0.0f, 1.0f);

    const float periodSeconds = 1.0f / speedHz;
    const uint32_t periodMicros = static_cast<uint32_t>(periodSeconds * 1e6f);

    mTimer.start(periodMicros);
    enterState(State::Playback);
}

void SmoothLed::stop(){
    if(mState == State::Static){
        return;
    }
    enterState(State::Static);
}

void SmoothLed::tick(){
    if(!mInitialized){
        TriggerDebug("SmoothLed", "not initialized");
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
            TriggerDebug("SmoothLed", "tick() unhandled state");
            break;
    }
}

void SmoothLed::enterState(State state){
    mState = state;
    if(state == State::Static){
        mActiveFunction = nullptr;
        mTimer.reset();
    }
}

void SmoothLed::handleSmoothingState(){
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

void SmoothLed::handlePlaybackState(){
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

SmoothLed::State SmoothLed::getState() const {
    return mState;
}

} // namespace Garbox
