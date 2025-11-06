#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "util/threading/LockGuard.h"
#include "SmoothLed.h"

namespace Garbox {

class SmoothLedAsync : public SmoothLed {
public:
    explicit SmoothLedAsync(LedcChannel& ledcChannel);

    void init();

    void setDefaultFunction(const FunctionIfc& fn);
    void setBrightness(float brightness);
    void setBrightnessSmooth(float brightness, float speedBrightnessPerSec, const FunctionIfc* fn = nullptr);
    void setPlayback(const FunctionIfc& fn, uint32_t cycles = 0, float speedHz = 1.0f, float minBrightness = 0.0f, float maxBrightness = 1.0f);

    void stop();
    void tick();

private:
    SemaphoreHandle_t mMutex = nullptr;
};

} // namespace Garbox
