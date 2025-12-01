#pragma once

#include <cstdint>
#include <new>

#include "core/assert/Assert.h"

namespace Garbox {

template <typename T>
class HeapStorage {
public:

    HeapStorage(std::size_t elementCount):
        // initialize members
        mBytes(0),
        mCapacity(elementCount),
        mCapacityBytes(elementCount * sizeof(T)){

        // allocate aligned raw storage on heap
        if(elementCount > 0){
            mBytes = new (std::nothrow) uint8_t[elementCount * sizeof(T)];
            const bool allocFailed = (mBytes == 0);
            AssertExit(!allocFailed, "HeapStorage", "failed to allocate heap memory");
        }
    }

    ~HeapStorage(){
        // heap-using class must never be destructed at runtime
        TriggerExit("HeapStorage", "heap using classes must not be deconstructed");
    }

    uint8_t* dataBytes() {
        return mBytes;
    }

    const uint8_t* dataBytes() const {
        return mBytes;
    }

    T* dataElements() {
        return reinterpret_cast<T*>(mBytes);
    }

    const T* dataElements() const {
        return reinterpret_cast<const T*>(mBytes);
    }

    T* elementPtr(std::size_t index) {
        return reinterpret_cast<T*>(mBytes + index * sizeof(T));
    }

    const T* elementPtr(std::size_t index) const {
        return reinterpret_cast<const T*>(mBytes + index * sizeof(T));
    }

    std::size_t capacityElements() const {
        return mCapacity;
    }

    std::size_t capacityBytes() const {
        return mCapacityBytes;
    }

private:
    uint8_t* mBytes;
    std::size_t mCapacity;
    std::size_t mCapacityBytes;
};

} // namespace Garbox
