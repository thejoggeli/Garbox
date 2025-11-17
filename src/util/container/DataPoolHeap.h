#pragma once

#include <cstddef>
#include <stdint.h>

#include "assert/Assert.h"

namespace Garbox {

class DataPoolHeap {
public:

    DataPoolHeap();
    ~DataPoolHeap();

    void init(size_t capacityBytes);
    void clear();

    void* allocateRaw(size_t sizeBytes, size_t alignmentBytes);

    template<typename T>
    T* allocate() {
        return static_cast<T*>(allocateRaw(sizeof(T), alignof(T)));
    }

    template<typename T>
    T* allocate(const T& data){
        T* ptr = allocate<T>();
        *ptr = data;
        return ptr;
    }

    size_t capacity() const {
        return mCapacityBytes;
    }

    size_t used() const {
        return mOffsetBytes;
    }

private:
    size_t mCapacityBytes = 0;
    uint8_t* mBuffer = nullptr;
    size_t mOffsetBytes = 0;
    bool mInitialized = false;
};

} // namespace Garbox
