#include "SmoothLedAsync.h"

#include "assert/Assert.h"

namespace Garbox {

SmoothLedAsync::SmoothLedAsync(LedcChannel& ledcChannel) : SmoothLed(ledcChannel){
    // nothing to do
}

void SmoothLedAsync::init(){
    AssertExit(!mMutex, "SmoothLedAsync", "already initialized");

    mMutex = xSemaphoreCreateRecursiveMutex();
    AssertExit(mMutex != nullptr, "SmoothLedAsync", "mutex creation failed");

    SmoothLed::init();
}

void SmoothLedAsync::setDefaultFunction(const FunctionIfc& fn){
    LockGuard guard(mMutex);
    SmoothLed::setDefaultFunction(fn);
}

void SmoothLedAsync::setBrightness(float brightness){
    LockGuard guard(mMutex);
    SmoothLed::setBrightness(brightness);
}

void SmoothLedAsync::setBrightnessSmooth(float brightness, float speedBrightnessPerSec, const FunctionIfc* fn){
    LockGuard guard(mMutex);
    SmoothLed::setBrightnessSmooth(brightness, speedBrightnessPerSec, fn);
}

void SmoothLedAsync::setPlayback(const FunctionIfc& fn, uint32_t cycles, float speedHz, float minBrightness, float maxBrightness){
    LockGuard guard(mMutex);
    SmoothLed::setPlayback(fn, cycles, speedHz, minBrightness, maxBrightness);
}

void SmoothLedAsync::stop(){
    LockGuard guard(mMutex);
    SmoothLed::stop();
}

void SmoothLedAsync::tick(){
    LockGuard guard(mMutex);
    SmoothLed::tick();
}

} // namespace Garbox
