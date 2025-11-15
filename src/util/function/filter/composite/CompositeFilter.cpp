#include "CompositeFilter.h"

#include "assert/Assert.h"

namespace Garbox {

CompositeFilter::CompositeFilter(FunctionIfc** transforms, float* results, uint32_t count):
    // initialize members
    mTransforms(transforms),
    mResults(results),
    mCount(count){

    // constructor body
    
    AssertExit((mTransforms != nullptr), "CompositeFilter", "null transform list");
    AssertExit((mCount > 0), "CompositeFilter", "no transforms provided");
    mHeapAllocated = false;
}


CompositeFilter::CompositeFilter(std::initializer_list<FunctionIfc*> transforms, bool storeResults):
    // initialize members
    mCount(static_cast<uint32_t>(transforms.size())){

    // constructor body
    AssertExit((mCount > 0), "CompositeFilter", "empty transform list");

    mTransforms = static_cast<FunctionIfc**>(std::calloc(mCount, sizeof(FunctionIfc*)));
    AssertExit((mTransforms != nullptr), "CompositeFilter", "heap allocation failed");

    uint32_t i = 0;
    for(FunctionIfc* t : transforms){
        mTransforms[i++] = t;
    }

    if(storeResults){
        mResults = static_cast<float*>(std::calloc(mCount, sizeof(float)));
    }

    mHeapAllocated = true;
}

CompositeFilter::~CompositeFilter(){
    if(mHeapAllocated){
        TriggerExit("CompositeFilter", "heap using classes must not be deconstructed");
    }
}

float CompositeFilter::onProcess(float value){

    // skip computation if delta below epsilon
    if(mInputThreshold != 0.0f && (std::fabs(value - mLastRaw) < mInputThreshold)){
        return mLastFiltered;
    }

    float result = value;

    // traverse transforms and store results in array
    if(mResults != nullptr){
        for(uint32_t i = 0; i < mCount; ++i){
            result = mTransforms[i]->apply(result);
            mResults[i] = result;
        }
    }
    // traverse transforms without storing
    else {
        for(uint32_t i = 0; i < mCount; ++i){
            result = mTransforms[i]->apply(result);
        }
    }

    return result;
}

void CompositeFilter::setInputThreshold(float threshold){
    mInputThreshold = std::max(0.0f, threshold);
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
