#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace Garbox {

class LockGuard {
public:
    explicit LockGuard(SemaphoreHandle_t mutex);
    ~LockGuard();

    // Non-copyable
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;

private:
    SemaphoreHandle_t mMutex;
    bool mLocked;
};

} // namespace Garbox
