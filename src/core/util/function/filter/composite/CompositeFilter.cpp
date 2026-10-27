#include "CompositeFilter.h"

#include "core/assert/Assert.h"

namespace Garbox {

CompositeFilter::CompositeFilter(FunctionIfc** transforms, float* results, uint32_t count):
    // initialize members
    mTransforms(transforms),
    mResults(results),
    mCount(count){

    // constructor body
    
    AssertExit((mTransforms != nullptr), "CompositeFilter", "null transform list");
    AssertExit((mCount > 0), "CompositeFilter", "no transforms provided");
    if(mTransforms == nullptr || mCount == 0){
        mTransforms = nullptr;
        mResults = nullptr;
        mCount = 0;
        return;
    }
    for(uint32_t index = 0; index < mCount; ++index){
        AssertExit(mTransforms[index] != nullptr, "CompositeFilter", "null transform");
    }
    mHeapAllocated = false;
}


CompositeFilter::CompositeFilter(std::initializer_list<FunctionIfc*> transforms, bool storeResults):
    // initialize members
    mCount(static_cast<uint32_t>(transforms.size())){

    // constructor body
    AssertExit((mCount > 0), "CompositeFilter", "empty transform list");
    if(mCount == 0){
        return;
    }

    mTransforms = static_cast<FunctionIfc**>(std::calloc(mCount, sizeof(FunctionIfc*)));
    AssertExit((mTransforms != nullptr), "CompositeFilter", "heap allocation failed");
    if(mTransforms == nullptr){
        mCount = 0;
        return;
    }

    mHeapAllocated = true;

    uint32_t i = 0;
    for(FunctionIfc* t : transforms){
        AssertExit(t != nullptr, "CompositeFilter", "null transform");
        mTransforms[i++] = t;
    }

    if(storeResults){
        mResults = static_cast<float*>(std::calloc(mCount, sizeof(float)));
        AssertExit(mResults != nullptr, "CompositeFilter", "result allocation failed");
    }
}

CompositeFilter::~CompositeFilter(){
    if(mHeapAllocated){
        std::free(mResults);
        std::free(mTransforms);
    }
}

float CompositeFilter::onProcess(float value){

    // skip computation if delta below epsilon
    if(mInputThreshold != 0.0f && (std::fabs(value - mLastRaw) < mInputThreshold)){
        return mLastFiltered;
    }

    float result = value;

    if(mCount == 0 || mTransforms == nullptr){
        return result;
    }

    // traverse transforms and store results in array
    if(mResults != nullptr){
        for(uint32_t i = 0; i < mCount; ++i){
            if(mTransforms[i] == nullptr){
                AssertExit(false, "CompositeFilter", "null transform");
                continue;
            }
            result = mTransforms[i]->apply(result);
            mResults[i] = result;
        }
    }
    // traverse transforms without storing
    else {
        for(uint32_t i = 0; i < mCount; ++i){
            if(mTransforms[i] == nullptr){
                AssertExit(false, "CompositeFilter", "null transform");
                continue;
            }
            result = mTransforms[i]->apply(result);
        }
    }

    return result;
}

void CompositeFilter::setInputThreshold(float threshold){
    const bool valid = std::isfinite(threshold) && threshold >= 0.0f;
    AssertExit(valid, "CompositeFilter", "invalid input threshold");
    if(valid){
        mInputThreshold = threshold;
    }
}

float CompositeFilter::getInputThreshold() const {
    return mInputThreshold;
}

float CompositeFilter::getResult(size_t index) const {
    if(mResults == nullptr){
        TriggerDebug("CompositeFilters", "results is nullptr");
        return 0.0f;
    }
    if(index >= mCount){
        TriggerDebug("CompositeFilters", "invalid results index");
        return 0.0f;
    }
    return mResults[index];
}

} // namespace Garbox
