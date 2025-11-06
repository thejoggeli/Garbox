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
    void setPlayback(const FunctionIfc& fn, uint32_t cycles = 0, uint32_t periodMicros = 1'000'000, float minBrightness = 0.0f, float maxBrightness = 1.0f);

    void stop();
    void tick();

private:
    SemaphoreHandle_t mMutex = nullptr;
};

} // namespace Garbox
