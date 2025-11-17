#pragma once

#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace Garbox {

class Task {
public:

    using TaskHandler = void(*)(void*);

    Task();

    void configure(
        const char* name,
        uint32_t stackSize,
        UBaseType_t priority,
        BaseType_t coreId
    );

    void start(TaskHandler handler, void* context);
    void stop();

    bool isRunning() const;
    bool isConfigured() const;
    bool isCurrentTask() const;
    TaskHandle_t getHandle();

private:
    const char* mName = nullptr;
    uint32_t mStackSize = 0;
    UBaseType_t mPriority = 0;
    BaseType_t mCoreId = 0;

    TaskHandler mTaskHandler = nullptr;
    void* mContext = nullptr;
    TaskHandle_t mTaskHandle = nullptr;
    bool mConfigured = false;

    static void handleTask(void* arg);
};

} // namespace Garbox
