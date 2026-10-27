#include "MovingAverageFilter.h"

#include "core/assert/Assert.h"
#include <cstdlib>
#include <cmath>
#include <limits>

namespace Garbox {

MovingAverageFilter::MovingAverageFilter(int32_t* buffer, uint32_t size, float scaleFactor):
    // initialize members
    mBuffer(buffer),
    mSize(size){
    // constructor body

    // validate buffer
    const bool validBuffer = (mBuffer != nullptr) && (mSize > 0);
    AssertExit(validBuffer, "MovingAverageFilter", "invalid external buffer");
    if(!validBuffer){
        mBuffer = &mFallbackBuffer;
        mSize = 1;
    }

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
    if(mSize == 0){
        mBuffer = &mFallbackBuffer;
        mSize = 1;
        setScaleFactor(scaleFactor);
        return;
    }
    mBuffer = static_cast<int32_t*>(std::calloc(mSize, sizeof(int32_t)));
    AssertExit((mBuffer != nullptr), "MovingAverageFilter", "buffer allocation failed");
    if(mBuffer == nullptr){
        mBuffer = &mFallbackBuffer;
        mSize = 1;
        setScaleFactor(scaleFactor);
        return;
    }
    
    // initialize scale factor
    setScaleFactor(scaleFactor);

    // heap was allocated
    mHeapAllocated = true;
}

MovingAverageFilter::~MovingAverageFilter(){
    if(mHeapAllocated){
        std::free(mBuffer);
    }
}

void MovingAverageFilter::setScaleFactor(float scaleFactor){
    const bool valid = std::isfinite(scaleFactor) && (scaleFactor > 0.0f);
    AssertExit(valid, "MovingAverageFilter", "invalid scale factor");
    if(!valid){
        return;
    }

    if(mCount > 0 && scaleFactor != mScaleFactor){
        for(uint32_t index = 0; index < mCount; ++index){
            const double value = static_cast<double>(mBuffer[index]) /
                                 static_cast<double>(mScaleFactor);
            const double scaled = value * static_cast<double>(scaleFactor);
            const bool representable =
                std::isfinite(scaled) &&
                scaled >= static_cast<double>(std::numeric_limits<int32_t>::min()) &&
                scaled <= static_cast<double>(std::numeric_limits<int32_t>::max());
            AssertExit(representable, "MovingAverageFilter", "rescaled value out of range");
            if(!representable){
                return;
            }
        }

        int64_t newSum = 0;
        for(uint32_t index = 0; index < mCount; ++index){
            const double value = static_cast<double>(mBuffer[index]) /
                                 static_cast<double>(mScaleFactor);
            const double scaled = value * static_cast<double>(scaleFactor);
            mBuffer[index] = static_cast<int32_t>(std::lround(scaled));
            newSum += mBuffer[index];
        }
        mSum = newSum;
    }
    mScaleFactor = scaleFactor;
}

void MovingAverageFilter::onReset(float value){
    int32_t valueInt = 0;
    if(!quantize(value, valueInt)){
        value = 0.0f;
        mLastRaw = value;
        mLastFiltered = value;
    }
    mBuffer[0] = valueInt;
    mSum = valueInt;
    mCount = 1;
    mCountFloat = static_cast<float>(mCount);
    mIndex = 0;
    mLastFiltered = value;
}

float MovingAverageFilter::onProcess(float value){

    int32_t valueInt = 0;
    if(!quantize(value, valueInt)){
        return mLastFiltered;
    }

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

bool MovingAverageFilter::quantize(float value, int32_t& quantized) const {
    const double scaled = static_cast<double>(value) * static_cast<double>(mScaleFactor);
    const bool representable =
        std::isfinite(scaled) &&
        scaled >= static_cast<double>(std::numeric_limits<int32_t>::min()) &&
        scaled <= static_cast<double>(std::numeric_limits<int32_t>::max());
    AssertExit(representable, "MovingAverageFilter", "input is not representable");
    if(!representable){
        quantized = 0;
        return false;
    }
    quantized = static_cast<int32_t>(std::lround(scaled));
    return true;
}

} // namespace Garbox
