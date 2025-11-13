#pragma once

#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace Garbox {

class PeriodicTask {
public:
    using Handler = void(*)(void*);

    PeriodicTask();
    ~PeriodicTask();

    void configure(
        const char* name,
        uint32_t frequencyHz,
        uint32_t stackSize,
        UBaseType_t priority,
        BaseType_t coreId
    );

    void start(Handler handler, void* context);
    void stop();

    bool isRunning() const;
    bool isConfigured() const;
    TaskHandle_t& getHandle();

private:
    const char* mName = nullptr;
    uint32_t mFrequencyHz = 0;
    uint32_t mPeriodMillis = 0;
    uint32_t mStackSize = 0;
    UBaseType_t mPriority = 0;
    BaseType_t mCoreId = 0;

    Handler mHandler = nullptr;
    void* mContext = nullptr;
    TaskHandle_t mTaskHandle = nullptr;
    bool mConfigured = false;

    static void taskLoop(void* arg);
};

} // namespace Garbox
