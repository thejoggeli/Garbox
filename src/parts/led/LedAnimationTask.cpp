#include "LedAnimationTask.h"

#include "core/diagnostics/Profiler.h"
#include "core/time/Time.h"
#include "global/AppConfig.h"

namespace Garbox {

LedAnimationTask::LedAnimationTask(uint8_t maxLeds):
    // init members
    mFrequencyHz(AppConfig::LedAnimationTaskFrequencyHz){
    AssertExit(maxLeds > 0, "LedAnimationTask", "maxLeds must be > 0");

    // reserve LED vector capacity once
    mLeds.reserve(maxLeds);
}

LedAnimationTask::~LedAnimationTask(){
    TriggerExit("LedAnimationTask", "heap using classes must not be deconstructed");
}

void LedAnimationTask::start(){
    AssertExit(mFrequencyHz > 0, "LedAnimationTask", "frequency must be > 0");
    AssertExit(mTaskHandle == nullptr, "LedAnimationTask", "already started");

    mPeriodMillis = static_cast<uint32_t>(1000.0f / static_cast<float>(mFrequencyHz));

    BaseType_t result = xTaskCreatePinnedToCore(
        taskLoop,
        AppConfig::LedAnimationTaskName,
        AppConfig::LedAnimationTaskStackSize,
        this,
        AppConfig::LedAnimationTaskPriority,
        &mTaskHandle,
        AppConfig::LedAnimationTaskCore
    );

    AssertExit(result == pdPASS, "LedAnimationTask", "task creation failed");
}

void LedAnimationTask::registerLed(AnimatedLedAsync* led){
    if(led == nullptr){
        TriggerExit("LedAnimationTask", "null LED registration");
        return;
    }
    if(mLeds.size() >= mLeds.capacity()){
        TriggerExit("LedAnimationTask", "max LED count reached");
        return;
    }
    mLeds.push_back(led);
}

void LedAnimationTask::tick(){
    Profiler::Scoped(ProfilerConfig::LedAnimationTick);
    for(auto* led : mLeds){
        if(led != nullptr){
            led->tick();
        }
    }
}

void LedAnimationTask::taskLoop(void* arg){
    LedAnimationTask* self = static_cast<LedAnimationTask*>(arg);
    while(true){
        self->tick();
        vTaskDelay(pdMS_TO_TICKS(self->mPeriodMillis));
    }
}

} // namespace Garbox
