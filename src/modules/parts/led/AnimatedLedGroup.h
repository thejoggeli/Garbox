#pragma once

#include <cstdint>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "core/rtos/Task.h"
#include "modules/parts/led/single/AnimatedLed.h"
#include "core/util/container/Span.h"

namespace Garbox {

class AnimatedLedGroup {
public:

    AnimatedLedGroup(Span<AnimatedLed> leds);
    ~AnimatedLedGroup();

    void init();
    void tick();

    void startTask(const char* taskName, uint32_t frequencyHz, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void stopTask();
    TaskHandle_t getTaskHandle() const;

    AnimatedLed& getLed(uint8_t index);
    Garbox::Span<AnimatedLed>& getAllLeds();

    void setLed(uint8_t index, float brightness);
    void setAllLeds(float brightness);
    void setAllLedsSmooth(float brightness, uint32_t durationMicros);

    void toggleLed(uint8_t index);
    void toggleAllLeds();
    void toggleAllLedsSmooth(uint32_t durationMicros);

    bool isInitialized() const;
    uint16_t getCount() const;

    // Disallow copy and move 
    AnimatedLedGroup(const AnimatedLedGroup&) = delete;
    AnimatedLedGroup& operator=(const AnimatedLedGroup&) = delete;
    AnimatedLedGroup(AnimatedLedGroup&&) = delete;
    AnimatedLedGroup& operator=(AnimatedLedGroup&&) = delete;

private:

    bool mInitialized = false;
    Span<AnimatedLed> mLeds;
    SemaphoreHandle_t mMutex = nullptr;

    Task mTask;
    uint32_t mTaskFrequencyHz = 0;

    void handleTask();

};

} // namespace Garbox
