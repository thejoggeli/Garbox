#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "freertos/FreeRTOS.h"

using TaskFunction_t = void (*)(void*);

namespace GarboxTest::FakeTask {

struct ControlBlock {
    TaskFunction_t function = nullptr;
    void* argument = nullptr;
    bool deleted = false;
};

inline std::array<ControlBlock, 16U> Tasks{};
inline std::size_t TaskCount = 0U;
inline ControlBlock* Current = nullptr;
inline TickType_t Ticks = 0U;
inline uint32_t SelfDeleteCount = 0U;
inline uint32_t ExternalDeleteCount = 0U;

inline void Reset(){
    Tasks = {};
    TaskCount = 0U;
    Current = nullptr;
    Ticks = 0U;
    SelfDeleteCount = 0U;
    ExternalDeleteCount = 0U;
}

inline void SetCurrent(ControlBlock* task){
    Current = task;
}

inline void Run(ControlBlock* task){
    Current = task;
    task->function(task->argument);
    Current = nullptr;
}

} // namespace GarboxTest::FakeTask

using TaskHandle_t = GarboxTest::FakeTask::ControlBlock*;

inline BaseType_t xTaskCreatePinnedToCore(TaskFunction_t function,
                                         const char*,
                                         uint32_t,
                                         void* argument,
                                         UBaseType_t,
                                         TaskHandle_t* handle,
                                         BaseType_t){
    using namespace GarboxTest::FakeTask;
    if(TaskCount >= Tasks.size() || handle == nullptr){
        return pdFALSE;
    }
    ControlBlock& task = Tasks[TaskCount++];
    task.function = function;
    task.argument = argument;
    task.deleted = false;
    *handle = &task;
    return pdPASS;
}

inline TaskHandle_t xTaskGetCurrentTaskHandle(){
    return GarboxTest::FakeTask::Current;
}

inline TickType_t xTaskGetTickCount(){
    return GarboxTest::FakeTask::Ticks;
}

inline void vTaskDelay(TickType_t ticks){
    GarboxTest::FakeTask::Ticks += ticks;
}

inline void vTaskDelete(TaskHandle_t task){
    using namespace GarboxTest::FakeTask;
    if(task == nullptr){
        ++SelfDeleteCount;
        if(Current != nullptr){
            Current->deleted = true;
        }
        return;
    }
    ++ExternalDeleteCount;
    task->deleted = true;
}
