#include "SmoothLedTask.h"

#include "core/time/Time.h"

namespace Garbox {

SmoothLedTask::SmoothLedTask(uint8_t maxLeds){
    AssertExit(maxLeds > 0, "SmoothLedTask", "maxLeds must be > 0");

    // reserve LED vector capacity once
    mLeds.reserve(maxLeds);
}

SmoothLedTask::~SmoothLedTask(){
    TriggerExit("SmoothLedTask", "heap using classes must not be deconstructed");
}

void SmoothLedTask::start(uint32_t frequencyHz){
    AssertExit(frequencyHz > 0, "SmoothLedTask", "frequency must be > 0");
    AssertExit(mTaskHandle == nullptr, "SmoothLedTask", "already started");

    mPeriodMillis = static_cast<uint32_t>(1000.0f / static_cast<float>(frequencyHz));

    BaseType_t result = xTaskCreatePinnedToCore(
        taskLoop,
        "SmoothLedTask",
        2048,
        this,
        2,
        &mTaskHandle,
        APP_CPU_NUM
    );

    AssertExit(result == pdPASS, "SmoothLedTask", "task creation failed");
}

void SmoothLedTask::registerLed(SmoothLedAsync* led){
    if(led == nullptr){
        TriggerDebug("SmoothLedTask", "null LED registration");
        return;
    }

    if(mLeds.size() >= mLeds.capacity()){
        TriggerDebug("SmoothLedTask", "max LED count reached");
        return;
    }

    mLeds.push_back(led);
}

void SmoothLedTask::taskLoop(void* arg){
    SmoothLedTask* self = static_cast<SmoothLedTask*>(arg);

    while(true){
        for(auto* led : self->mLeds){
            if(led != nullptr){
                led->tick();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(self->mPeriodMillis));
    }
}

} // namespace Garbox
