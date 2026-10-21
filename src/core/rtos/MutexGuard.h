#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace Garbox {

// Task-context RAII guard for a mutex created by xSemaphoreCreateMutex().
// Do not use from an ISR or with a recursive mutex.
class MutexGuard final {
public:
    explicit MutexGuard(SemaphoreHandle_t mutex);
    ~MutexGuard();

    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;

private:
    SemaphoreHandle_t mMutex;
    bool mLocked;
};

// Task-context RAII guard for a mutex created by xSemaphoreCreateRecursiveMutex().
// Use only when the same task can acquire the same mutex through nested calls.
class RecursiveMutexGuard final {
public:
    explicit RecursiveMutexGuard(SemaphoreHandle_t mutex);
    ~RecursiveMutexGuard();

    RecursiveMutexGuard(const RecursiveMutexGuard&) = delete;
    RecursiveMutexGuard& operator=(const RecursiveMutexGuard&) = delete;

private:
    SemaphoreHandle_t mMutex;
    bool mLocked;
};

} // namespace Garbox
