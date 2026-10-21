#pragma once

#include <atomic>
#include <cstdint>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <functional>

#include "core/rtos/TaskStopToken.h"

namespace Garbox {

class Task {
public:

    using TaskHandler = std::function<void(const TaskStopToken&)>;

    Task();
    ~Task();

    void configure(
        const char* name,
        uint32_t stackSize,
        UBaseType_t priority,
        BaseType_t coreId
    );


    void start();

    // Cooperative stop contract: the handler must observe its token and
    // return after releasing owned resources. No caller deletes another task.
    void requestStop();
    bool waitForStop(TickType_t timeoutTicks);
    bool stop(TickType_t timeoutTicks = pdMS_TO_TICKS(1500));

    void setHandler(TaskHandler handler);

    bool isRunning() const;
    bool isConfigured() const;
    bool isCurrentTask() const;
    TaskHandle_t getHandle() const;

private:
    const char* mName = nullptr;
    uint32_t mStackSize = 0;
    UBaseType_t mPriority = 0;
    BaseType_t mCoreId = 0;

    TaskHandler mTaskHandler = nullptr;
    void* mContext = nullptr;
    std::atomic<TaskHandle_t> mTaskHandle{nullptr};
    bool mConfigured = false;
    TaskStopSource mStopSource;

    static void handleTask(void* arg);
};

} // namespace Garbox
