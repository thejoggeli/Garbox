#include "AnimatedLedGroup.h"

#include "core/assert/Assert.h"
#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/util/threading/LockGuard.h"

namespace Garbox {

AnimatedLedGroup::AnimatedLedGroup(Span<AnimatedLed> leds) : mLeds(leds){
    mMutex = xSemaphoreCreateMutex();
    AssertExit((mMutex != nullptr), "AnimatedLedGroup", "mutex create failed");
}

AnimatedLedGroup::~AnimatedLedGroup(){
    TriggerExit("AnimatedLedGroup", "heap using classes must not be deconstructed");
}

void AnimatedLedGroup::init(){
    LockGuard lock(mMutex);
    AssertExit(!mInitialized, "AnimatedLedGroup", "already initialized");

    // init leds
    for(AnimatedLed& led : mLeds){
        led.init();
    }

    // task handler
    mTask.setHandler([this](){
        this->handleTask();
    });

    mInitialized = true;
}

void AnimatedLedGroup::startTask(const char* taskName, uint32_t frequencyHz, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId){
    LockGuard lock(mMutex);

    // check state
    AssertExit(mInitialized, "AnimatedLedGroup", "not initialized");
    AssertExit(frequencyHz > 0, "AnimatedLedGroup", "task frequency must be > 0");
    
    // set before task starts!
    mTaskFrequencyHz = frequencyHz;

    // start task
    mTask.configure(taskName, stackSize, priority, coreId);
    mTask.start();
}

void AnimatedLedGroup::stopTask(){
    LockGuard lock(mMutex);
    mTask.stop();
}

TaskHandle_t AnimatedLedGroup::getTaskHandle() const {
    return mTask.getHandle();
}

void AnimatedLedGroup::handleTask(){
    
    // task timing
    const uint32_t periodMillis = 1000.0f / mTaskFrequencyHz;
    const TickType_t periodTicks = pdMS_TO_TICKS(periodMillis);
    TickType_t lastWake = xTaskGetTickCount();

    // loop task forever
    while(true){
        tick();
        vTaskDelayUntil(&lastWake, periodTicks);
    }
}

void AnimatedLedGroup::tick(){
    LockGuard lock(mMutex);
    Profiler::Scoped(ProfilerId::LedAnimationTick);

    // must be initialized
    if(!mInitialized){
        TriggerDebug("AnimatedLedGroup", "not initialized");
        return;
    }

    // tick animated leds
    for(AnimatedLed& led : mLeds){
        led.tick();
    }
}

AnimatedLed& AnimatedLedGroup::getLed(uint8_t index){
    if(index >= mLeds.size()){
        TriggerExit("AnimatedLedGroup", "invalid animated led index");
    }
    return mLeds[index];
}

Span<AnimatedLed>& AnimatedLedGroup::getAllLeds(){
    return mLeds;
}

void AnimatedLedGroup::setLed(uint8_t index, float brightness){
    LockGuard lock(mMutex);

    if(!mInitialized){
        TriggerDebug("AnimatedLedGroup", "not initialized");
        return;
    }

    if(index >= mLeds.size()){
        TriggerDebug("AnimatedLedGroup", "invalid animated led index");
        return;
    }

    mLeds[index].setBrightness(brightness);
}

void AnimatedLedGroup::setAllLeds(float brightness){
    LockGuard lock(mMutex);

    if(!mInitialized){
        TriggerDebug("AnimatedLedGroup", "not initialized");
        return;
    }

    for(AnimatedLed& led : mLeds){
        led.setBrightness(brightness);
    }
}

void AnimatedLedGroup::setAllLedsSmooth(float brightness, uint32_t durationMicros){
    LockGuard lock(mMutex);

    if(!mInitialized){
        TriggerDebug("AnimatedLedGroup", "not initialized");
        return;
    }

    for(AnimatedLed& led : mLeds){
        led.setBrightnessSmooth(brightness, durationMicros);
    }
}

void AnimatedLedGroup::toggleLed(uint8_t index){
    LockGuard lock(mMutex);

    if(!mInitialized){
        TriggerDebug("AnimatedLedGroup", "not initialized");
        return;
    }

    if(index >= mLeds.size()){
        TriggerDebug("AnimatedLedGroup", "invalid animated led index");
        return;
    }

    if(mLeds[index].getBrightness() >= 0.5f){
        mLeds[index].setBrightness(0.0f);
    }
    else {
        mLeds[index].setBrightness(1.0f);
    }
}

void AnimatedLedGroup::toggleAllLeds(){
    LockGuard lock(mMutex);

    if(!mInitialized){
        TriggerDebug("AnimatedLedGroup", "not initialized");
        return;
    }

    for(AnimatedLed& led : mLeds){
        if(led.getBrightness() >= 0.5f){
            led.setBrightness(0.0f);
        }
        else {
            led.setBrightness(1.0f);
        }
    }
}

void AnimatedLedGroup::toggleAllLedsSmooth(uint32_t durationMicros){
    LockGuard lock(mMutex);

    if(!mInitialized){
        TriggerDebug("AnimatedLedGroup", "not initialized");
        return;
    }

    for(AnimatedLed& led : mLeds){
        if(led.getBrightness() > 0.5f){
            led.setBrightnessSmooth(0.0f, durationMicros);
        }
        else {
            led.setBrightnessSmooth(1.0f, durationMicros);
        }
    }
}

bool AnimatedLedGroup::isInitialized() const {
    return mInitialized;
}

uint16_t AnimatedLedGroup::getCount() const {
    return mLeds.size();
}

} // namespace Garbox
