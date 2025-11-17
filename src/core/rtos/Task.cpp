#include "Task.h"

#include "assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox {

Task::Task(){
    // nothing to do
}

void Task::configure(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId){
    AssertExit(name != nullptr, "Task", "null name");
    mName = name;
    mStackSize = stackSize;
    mPriority = priority;
    mCoreId = coreId;
    mConfigured = true;
}

void Task::start(TaskHandler handler, void* context){
    AssertExit(mConfigured, "Task", "not configured");
    AssertExit(handler != nullptr, "Task", "null handler");
    AssertExit(mTaskHandle == nullptr, "Task", "task already running");

    mTaskHandler = handler;
    mContext = context;

    BaseType_t result = xTaskCreatePinnedToCore(
        handleTask,
        mName,
        mStackSize,
        this,
        mPriority,
        &mTaskHandle,
        mCoreId
    );

    AssertExit(result == pdPASS, "Task", "task creation failed");
}

void Task::stop(){
    if(mTaskHandle == nullptr){
        TriggerDebug("Task", "stop failed, task is not running");
        LogWarning("Task", "%s is not running", mName);
        return;
    }
    if(xTaskGetCurrentTaskHandle() == mTaskHandle){
        TriggerDebug("Task", "task not allowed to stop itself");
        LogWarning("Task", "%s tried to stop itself", mName);
        return;
    }
    vTaskDelete(mTaskHandle);
    mTaskHandle = nullptr;
    LogDebug("Task", "%s stopped", mName);
}

bool Task::isRunning() const {
    return (mTaskHandle != nullptr);
}

bool Task::isConfigured() const {
    return mConfigured;
}

bool Task::isCurrentTask() const {
    return (mTaskHandle == xTaskGetCurrentTaskHandle());
}

void Task::handleTask(void* arg){
    Task* self = static_cast<Task*>(arg);
    LogDebug("Task", "%s started", self->mName);
    self->mTaskHandler(self->mContext);
    TriggerExit("Task", "reach end of handler");
}

TaskHandle_t Task::getHandle() const {
    return mTaskHandle;
}

} // namespace Garbox
