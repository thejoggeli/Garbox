#include "DataPoolHeap.h"

namespace Garbox {

DataPoolHeap::DataPoolHeap(size_t capacityBytes) : mCapacityBytes(capacityBytes) {
    mBuffer = new uint8_t[mCapacityBytes];
    AssertExit(mBuffer != nullptr, "DataPoolHeap", "heap allocation failed");
}

DataPoolHeap::~DataPoolHeap() {
    AssertExit(mCapacityBytes == 0, "DataPoolHeap", "heap using classes must not be deconstructed");
}

void DataPoolHeap::clear() {
    mOffsetBytes = 0;
}

void* DataPoolHeap::allocateRaw(size_t sizeBytes, size_t alignmentBytes) {
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
