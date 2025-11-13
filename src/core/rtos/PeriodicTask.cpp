#include "PeriodicTask.h"

#include "assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/time/Time.h"

namespace Garbox {

PeriodicTask::PeriodicTask(){
    // nothing to do
}

PeriodicTask::~PeriodicTask(){
    TriggerExit("PeriodicTask", "heap using classes must not be deconstructed");
}

void PeriodicTask::configure(const char* name, uint32_t frequencyHz, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId){
    AssertExit(frequencyHz > 0, "PeriodicTask", "frequency must be > 0");
    AssertExit(name != nullptr, "PeriodicTask", "null name");

    mName = name;
    mFrequencyHz = frequencyHz;
    mPeriodMillis = static_cast<uint32_t>(1000.0f / static_cast<float>(frequencyHz));
    mStackSize = stackSize;
    mPriority = priority;
    mCoreId = coreId;

    mConfigured = true;
}

void PeriodicTask::start(Handler handler, void* context){
    AssertExit(mConfigured, "PeriodicTask", "not configured");
    AssertExit(handler != nullptr, "PeriodicTask", "null handler");
    AssertExit(mTaskHandle == nullptr, "PeriodicTask", "task already running");

    mHandler = handler;
    mContext = context;

    BaseType_t result = xTaskCreatePinnedToCore(
        taskLoop,
        mName,
        mStackSize,
        this,
        mPriority,
        &mTaskHandle,
        mCoreId
    );

    AssertExit(result == pdPASS, "PeriodicTask", "task creation failed");
}

void PeriodicTask::stop(){
    if(mTaskHandle != nullptr){
        vTaskDelete(mTaskHandle);
        mTaskHandle = nullptr;
    }
}

bool PeriodicTask::isRunning() const{
    return (mTaskHandle != nullptr);
}

bool PeriodicTask::isConfigured() const{
    return mConfigured;
}

void PeriodicTask::taskLoop(void* arg){
    PeriodicTask* self = static_cast<PeriodicTask*>(arg);
    AssertExit(self != nullptr, "PeriodicTask", "invalid context");

    while(true){
        if(self->mHandler != nullptr){
            self->mHandler(self->mContext);
        }
        vTaskDelay(pdMS_TO_TICKS(self->mPeriodMillis));
    }
}

} // namespace Garbox
