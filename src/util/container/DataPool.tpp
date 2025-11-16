#include "DataPool.h"

namespace Garbox {

template<size_t CapacityBytes>
DataPool<CapacityBytes>::DataPool() {
    // nothing to do
}

template<size_t CapacityBytes>
DataPool<CapacityBytes>::~DataPool() {
    // nothing to do
}

template<size_t CapacityBytes>
void DataPool<CapacityBytes>::init() {
    AssertExit(!mInitialized, "DataPool", "already initialized");

    mOffsetBytes = 0;
    mInitialized = true;
}

template<size_t CapacityBytes>
void DataPool<CapacityBytes>::clear() {
    if(!mInitialized) {
        TriggerDebug("DataPool", "not initialized");
        return;
    }

    mOffsetBytes = 0;
}

template<size_t CapacityBytes>
void* DataPool<CapacityBytes>::allocateRaw(size_t sizeBytes, size_t alignmentBytes) {
    if(!mInitialized) {
        TriggerDebug("DataPool", "not initialized");
        return nullptr;
    }

    const size_t mask = alignmentBytes - 1;
    size_t alignedOffset = (mOffsetBytes + mask) & ~mask;

    const bool enoughSpace = (alignedOffset + sizeBytes) <= CapacityBytes;
    if(!enoughSpace) {
        TriggerDebug("DataPool", "insufficient space");
        return nullptr;
    }

    uint8_t* resultPtr = mBuffer.data() + alignedOffset;
    mOffsetBytes = alignedOffset + sizeBytes;

    return resultPtr;
}

} // namespace Garbox
