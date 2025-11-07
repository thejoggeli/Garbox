#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "util/threading/LockGuard.h"
#include "AnimatedLed.h"

namespace Garbox {

class AnimatedLedAsync : public AnimatedLed {
public:
    explicit AnimatedLedAsync(LedcChannel& ledcChannel);

    void init();

    void setDefaultFunction(const FunctionIfc& fn);
    void setBrightness(float brightness);
    void setBrightnessSmooth(float brightness, uint32_t durationMicros, const FunctionIfc* fn = nullptr);
    void setAnimation(const FunctionIfc& fn, uint32_t cycles, uint32_t durationMicros, float yStart=0.0f, float yEnd=1.0f);

    void animationAddFrame(const FunctionIfc& fn, uint32_t durationMicros, float yStart=0.0f, float yEnd=1.0f);
    void animationAddDelay(uint32_t durationMicros);
    void animationStart(uint32_t cycles = 1);
    void animationClear();

    void stop();
    void tick();

private:
    SemaphoreHandle_t mMutex = nullptr;
};

} // namespace Garbox
