#include "LockGuard.h"

namespace Garbox {

LockGuard::LockGuard(SemaphoreHandle_t mutex): mMutex(mutex), mLocked(false){
    if (mMutex != nullptr){
        if (xSemaphoreTakeRecursive(mMutex, portMAX_DELAY) == pdTRUE){
            mLocked = true;
        }
    }
}

LockGuard::~LockGuard(){
    if (mLocked && mMutex != nullptr){
        xSemaphoreGiveRecursive(mMutex);
    }
}

} // namespace Garbox
