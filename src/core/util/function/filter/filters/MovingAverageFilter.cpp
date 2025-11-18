#include "MovingAverageFilter.h"

#include "core/assert/Assert.h"
#include <cstdlib>
#include <cmath>

namespace Garbox {

MovingAverageFilter::MovingAverageFilter(int32_t* buffer, uint32_t size, float scaleFactor):
    // initialize members
    mBuffer(buffer),
    mSize(size){
    // constructor body

    // validate buffer
    AssertExit((mBuffer != nullptr), "MovingAverageFilter", "null buffer");
    AssertExit((mSize > 0), "MovingAverageFilter", "invalid buffer size");

    // initialize scale factor
    setScaleFactor(scaleFactor);

    // no heap was allocated
    mHeapAllocated = false;
}

MovingAverageFilter::MovingAverageFilter(uint32_t size, float scaleFactor):
    // initialize members
    mSize(size){
    // constructor body

    // allocate memory for buffer
    AssertExit((mSize > 0), "MovingAverageFilter", "invalid buffer size");
    mBuffer = static_cast<int32_t*>(std::calloc(mSize, sizeof(int32_t)));
    AssertExit((mBuffer != nullptr), "MovingAverageFilter", "buffer allocation failed");
    
    // initialize scale factor
    setScaleFactor(scaleFactor);

    // heap was allocated
    mHeapAllocated = true;
}

MovingAverageFilter::~MovingAverageFilter(){
    if(mHeapAllocated){
        TriggerExit("MovingAverageFilter", "heap using classes must not be deconstructed");
    }
}

void MovingAverageFilter::setScaleFactor(float scaleFactor){
    AssertExit((mScaleFactor > 0.0f), "MovingAverageFilter", "invalid scale factor");
    mScaleFactor = scaleFactor;
}

void MovingAverageFilter::onReset(float value){
    const int32_t valueInt = static_cast<int32_t>(value * mScaleFactor);
    mBuffer[0] = valueInt;
    mSum = valueInt;
    mCount = 1;
    mCountFloat = static_cast<float>(mCount);
    mIndex = 0;
    mLastFiltered = value;
}

float MovingAverageFilter::onProcess(float value){

    const int32_t valueInt = static_cast<int32_t>(value * mScaleFactor);

    if(mCount < mSize){
        mBuffer[mCount++] = valueInt;
        mSum += valueInt;
        mCountFloat = static_cast<float>(mCount);
    }
    else {
        mSum -= mBuffer[mIndex];
        mBuffer[mIndex] = valueInt;
        mSum += valueInt;
        mIndex += 1;
        if(mIndex >= mSize){
            mIndex = 0;
        }
    }

    return static_cast<float>(mSum) / (mCountFloat * mScaleFactor);
}

} // namespace Garbox
