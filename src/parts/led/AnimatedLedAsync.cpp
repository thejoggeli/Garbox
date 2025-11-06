#include "AnimatedLedAsync.h"

#include "assert/Assert.h"

namespace Garbox {

AnimatedLedAsync::AnimatedLedAsync(LedcChannel& ledcChannel) : AnimatedLed(ledcChannel){
    // nothing to do
}

void AnimatedLedAsync::init(){
    AssertExit(!mMutex, "AnimatedLedAsync", "already initialized");

    mMutex = xSemaphoreCreateRecursiveMutex();
    AssertExit(mMutex != nullptr, "AnimatedLedAsync", "mutex creation failed");

    AnimatedLed::init();
}

void AnimatedLedAsync::setDefaultFunction(const FunctionIfc& fn){
    LockGuard guard(mMutex);
    AnimatedLed::setDefaultFunction(fn);
}

void AnimatedLedAsync::setBrightness(float brightness){
    LockGuard guard(mMutex);
    AnimatedLed::setBrightness(brightness);
}

void AnimatedLedAsync::setBrightnessSmooth(float brightness, uint32_t durationMicros, const FunctionIfc* fn){
    LockGuard guard(mMutex);
    AnimatedLed::setBrightnessSmooth(brightness, durationMicros, fn);
}

void AnimatedLedAsync::setPlayback(const FunctionIfc& fn, uint32_t cycles, uint32_t periodMicros, float minBrightness, float maxBrightness){
    LockGuard guard(mMutex);
    AnimatedLed::setPlayback(fn, cycles, periodMicros, minBrightness, maxBrightness);
}

void AnimatedLedAsync::stop(){
    LockGuard guard(mMutex);
    AnimatedLed::stop();
}

void AnimatedLedAsync::tick(){
    LockGuard guard(mMutex);
    AnimatedLed::tick();
}

} // namespace Garbox
