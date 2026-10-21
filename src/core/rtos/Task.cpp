#include "Task.h"

#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox {

Task::Task(){
    // nothing to do
}

Task::~Task(){
    if(isRunning()){
        TriggerExit("Task", "destroyed while still running");
    }
}

void Task::configure(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId){
    AssertExit(name != nullptr, "Task", "null name");
    mName = name;
    mStackSize = stackSize;
    mPriority = priority;
    mCoreId = coreId;
    mConfigured = true;
}

void Task::start(){
    AssertExit(mConfigured, "Task", "not configured");
    AssertExit(mTaskHandle.load(std::memory_order_acquire) == nullptr,
               "Task", "task already running");
    AssertExit(mTaskHandler != nullptr, "Task", "handler not set");
    mStopSource.reset();
    TaskHandle_t taskHandle = nullptr;
    BaseType_t result = xTaskCreatePinnedToCore(
        handleTask,
        mName,
        mStackSize,
        this,
        mPriority,
        &taskHandle,
        mCoreId
    );

    AssertExit(result == pdPASS, "Task", "task creation failed");
    mTaskHandle.store(taskHandle, std::memory_order_release);
}

void Task::requestStop(){
    if(mTaskHandle.load(std::memory_order_acquire) == nullptr){
        return;
    }

    mStopSource.requestStop();
}

bool Task::waitForStop(TickType_t timeoutTicks){
    if(mTaskHandle.load(std::memory_order_acquire) == nullptr){
        return true;
    }
    if(isCurrentTask()){
        return false;
    }

    const TickType_t start = xTaskGetTickCount();
    while(mTaskHandle.load(std::memory_order_acquire) != nullptr){
        if(timeoutTicks != portMAX_DELAY
           && static_cast<TickType_t>(xTaskGetTickCount() - start) >= timeoutTicks){
            return false;
        }
        vTaskDelay(1);
    }
    return true;
}

bool Task::stop(TickType_t timeoutTicks){
    requestStop();
    return waitForStop(timeoutTicks);
}

void Task::setHandler(TaskHandler handler){
    mTaskHandler = handler;
}

bool Task::isRunning() const {
    return mTaskHandle.load(std::memory_order_acquire) != nullptr;
}

bool Task::isConfigured() const {
    return mConfigured;
}

bool Task::isCurrentTask() const {
    const TaskHandle_t taskHandle = mTaskHandle.load(std::memory_order_acquire);
    return taskHandle != nullptr && taskHandle == xTaskGetCurrentTaskHandle();
}

void Task::handleTask(void* arg){
    Task* self = static_cast<Task*>(arg);
    LogDebug("Task", "%s started", self->mName);
    self->mTaskHandler(self->mStopSource.token());
    if(!self->mStopSource.stopRequested()){
        TriggerExit("Task", "handler returned without stop request");
    }

    LogDebug("Task", "%s stopped", self->mName);
    self->mTaskHandle.store(nullptr, std::memory_order_release);
    vTaskDelete(nullptr);
}

TaskHandle_t Task::getHandle() const {
    return mTaskHandle.load(std::memory_order_acquire);
}

} // namespace Garbox
