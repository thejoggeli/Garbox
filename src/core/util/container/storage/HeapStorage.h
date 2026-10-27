#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>

#include "core/assert/Assert.h"

namespace Garbox {

template <typename T>
class HeapStorage {
public:

    static_assert(
        alignof(T) <= alignof(std::max_align_t),
        "HeapStorage does not support over-aligned element types"
    );

    HeapStorage(std::size_t elementCount):
        // initialize members
        mBytes(nullptr),
        mCapacity(0),
        mCapacityBytes(0){

        const bool sizeOverflow =
            elementCount > (std::numeric_limits<std::size_t>::max() / sizeof(T));
        if(sizeOverflow){
            TriggerExit("HeapStorage", "allocation size overflow");
            return;
        }

        mCapacity = elementCount;
        mCapacityBytes = elementCount * sizeof(T);

        // allocate aligned raw storage on heap
        if(mCapacityBytes > 0){
            mBytes = static_cast<uint8_t*>(::operator new(mCapacityBytes, std::nothrow));
            const bool allocFailed = (mBytes == nullptr);
            AssertExit(!allocFailed, "HeapStorage", "failed to allocate heap memory");
            if(allocFailed){
                mCapacity = 0;
                mCapacityBytes = 0;
            }
        }
    }

    ~HeapStorage(){
        ::operator delete(mBytes);
    }

    HeapStorage(const HeapStorage&) = delete;
    HeapStorage& operator=(const HeapStorage&) = delete;
    HeapStorage(HeapStorage&&) = delete;
    HeapStorage& operator=(HeapStorage&&) = delete;

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
