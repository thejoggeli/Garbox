#pragma once

#include <vector>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "assert/Assert.h"
#include "AnimatedLedAsync.h"

namespace Garbox {

class LedAnimationTask {
public:
    explicit LedAnimationTask(uint8_t maxLeds);
    ~LedAnimationTask();

    void start();
    void registerLed(AnimatedLedAsync* led);

private:
    std::vector<AnimatedLedAsync*> mLeds;
    uint32_t mFrequencyHz;
    uint32_t mPeriodMillis = 0;
    TaskHandle_t mTaskHandle = nullptr;

    void tick();
    static void taskLoop(void* arg);
};

} // namespace Garbox
