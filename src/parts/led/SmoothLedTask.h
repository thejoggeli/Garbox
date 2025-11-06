#pragma once

#include <vector>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "assert/Assert.h"
#include "SmoothLedAsync.h"

namespace Garbox {

class SmoothLedTask {
public:
    explicit SmoothLedTask(uint8_t maxLeds);
    ~SmoothLedTask();

    void start(uint32_t frequencyHz = 60);
    void registerLed(SmoothLedAsync* led);

private:
    std::vector<SmoothLedAsync*> mLeds;
    uint32_t mPeriodMillis = 0;
    TaskHandle_t mTaskHandle = nullptr;

    static void taskLoop(void* arg);
};

} // namespace Garbox
