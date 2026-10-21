#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "freertos/FreeRTOS.h"

namespace GarboxTest::FakeSemaphore {

enum class Kind : uint8_t {
    Normal,
    Recursive,
};

struct Semaphore {
    Kind kind = Kind::Normal;
    uint32_t depth = 0U;
};

struct CallCounts {
    uint32_t normalTake = 0U;
    uint32_t normalGive = 0U;
    uint32_t recursiveTake = 0U;
    uint32_t recursiveGive = 0U;
};

inline std::array<Semaphore, 16U> Semaphores{};
inline std::size_t SemaphoreCount = 0U;
inline CallCounts Calls{};

inline Semaphore* Create(Kind kind){
    if(SemaphoreCount >= Semaphores.size()){
        return nullptr;
    }
    Semaphore& semaphore = Semaphores[SemaphoreCount++];
    semaphore = Semaphore{kind, 0U};
    return &semaphore;
}

inline void ResetCallCounts(){
    Calls = {};
}

} // namespace GarboxTest::FakeSemaphore

using SemaphoreHandle_t = GarboxTest::FakeSemaphore::Semaphore*;

inline SemaphoreHandle_t xSemaphoreCreateMutex(){
    return GarboxTest::FakeSemaphore::Create(GarboxTest::FakeSemaphore::Kind::Normal);
}

inline SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(){
    return GarboxTest::FakeSemaphore::Create(GarboxTest::FakeSemaphore::Kind::Recursive);
}

inline BaseType_t xSemaphoreTake(SemaphoreHandle_t semaphore, TickType_t){
    ++GarboxTest::FakeSemaphore::Calls.normalTake;
    if(semaphore == nullptr
       || semaphore->kind != GarboxTest::FakeSemaphore::Kind::Normal
       || semaphore->depth != 0U){
        return pdFALSE;
    }
    semaphore->depth = 1U;
    return pdTRUE;
}

inline BaseType_t xSemaphoreGive(SemaphoreHandle_t semaphore){
    ++GarboxTest::FakeSemaphore::Calls.normalGive;
    if(semaphore == nullptr
       || semaphore->kind != GarboxTest::FakeSemaphore::Kind::Normal
       || semaphore->depth != 1U){
        return pdFALSE;
    }
    semaphore->depth = 0U;
    return pdTRUE;
}

inline BaseType_t xSemaphoreTakeRecursive(SemaphoreHandle_t semaphore, TickType_t){
    ++GarboxTest::FakeSemaphore::Calls.recursiveTake;
    if(semaphore == nullptr || semaphore->kind != GarboxTest::FakeSemaphore::Kind::Recursive){
        return pdFALSE;
    }
    ++semaphore->depth;
    return pdTRUE;
}

inline BaseType_t xSemaphoreGiveRecursive(SemaphoreHandle_t semaphore){
    ++GarboxTest::FakeSemaphore::Calls.recursiveGive;
    if(semaphore == nullptr
       || semaphore->kind != GarboxTest::FakeSemaphore::Kind::Recursive
       || semaphore->depth == 0U){
        return pdFALSE;
    }
    --semaphore->depth;
    return pdTRUE;
}
