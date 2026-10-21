#include "MutexGuard.h"

#include "core/assert/Assert.h"

namespace Garbox {

MutexGuard::MutexGuard(SemaphoreHandle_t mutex):
    mMutex(mutex),
    mLocked(false){
    AssertExit(mMutex != nullptr, "MutexGuard", "mutex is nullptr");
    if(mMutex != nullptr){
        mLocked = xSemaphoreTake(mMutex, portMAX_DELAY) == pdTRUE;
        AssertExit(mLocked, "MutexGuard", "failed to take mutex");
    }
}

MutexGuard::~MutexGuard(){
    if(mLocked){
        AssertExit(xSemaphoreGive(mMutex) == pdTRUE, "MutexGuard", "failed to give mutex");
    }
}

RecursiveMutexGuard::RecursiveMutexGuard(SemaphoreHandle_t mutex):
    mMutex(mutex),
    mLocked(false){
    AssertExit(mMutex != nullptr, "RecursiveMutexGuard", "mutex is nullptr");
    if(mMutex != nullptr){
        mLocked = xSemaphoreTakeRecursive(mMutex, portMAX_DELAY) == pdTRUE;
        AssertExit(mLocked, "RecursiveMutexGuard", "failed to take mutex");
    }
}

RecursiveMutexGuard::~RecursiveMutexGuard(){
    if(mLocked){
        AssertExit(xSemaphoreGiveRecursive(mMutex) == pdTRUE,
                   "RecursiveMutexGuard", "failed to give mutex");
    }
}

} // namespace Garbox
