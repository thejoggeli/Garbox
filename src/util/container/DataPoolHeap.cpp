#include "DataPoolHeap.h"

namespace Garbox {

DataPoolHeap::DataPoolHeap(size_t capacityBytes):
    // initialize members
    mCapacityBytes(capacityBytes) {
    // nothing to do
}

DataPoolHeap::~DataPoolHeap() {
    TriggerExit("DataPoolHeap", "heap using classes must not be deconstructed");
}

void DataPoolHeap::init() {
    AssertExit(!mInitialized, "DataPoolHeap", "already initialized");

    mBuffer = new uint8_t[mCapacityBytes];
    AssertExit(mBuffer != nullptr, "DataPoolHeap", "heap allocation failed");

    mOffsetBytes = 0;
    mInitialized = true;
}

void DataPoolHeap::clear() {
    if(!mInitialized) {
        TriggerDebug("DataPoolHeap", "not initialized");
        return;
    }

    mOffsetBytes = 0;
}

void* DataPoolHeap::allocateRaw(size_t sizeBytes, size_t alignmentBytes) {
    if(!mInitialized) {
        TriggerDebug("DataPoolHeap", "not initialized");
        return nullptr;
    }

    const size_t mask = alignmentBytes - 1;
    size_t alignedOffset = (mOffsetBytes + mask) & ~mask;

    const bool enoughSpace = (alignedOffset + sizeBytes) <= mCapacityBytes;
    if(!enoughSpace) {
        TriggerDebug("DataPoolHeap", "insufficient space");
        return nullptr;
    }

    uint8_t* resultPtr = mBuffer + alignedOffset;
    mOffsetBytes = alignedOffset + sizeBytes;

    return resultPtr;
}

} // namespace Garbox
