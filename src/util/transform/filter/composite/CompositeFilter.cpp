#include "CompositeFilter.h"

#include "assert/Assert.h"

namespace Garbox {

CompositeFilter::CompositeFilter(TransformIfc** transforms, uint32_t count):
    // initialize members
    mTransforms(transforms),
    mCount(count){

    // constructor body
    
    AssertExit((mTransforms != nullptr), "CompositeFilter", "null transform list");
    AssertExit((mCount > 0), "CompositeFilter", "no transforms provided");
    mHeapAllocated = false;
}

CompositeFilter::CompositeFilter(std::initializer_list<TransformIfc*> transforms):
    // initialize members
    mCount(static_cast<uint32_t>(transforms.size())){

    // constructor body
    AssertExit((mCount > 0), "CompositeFilter", "empty transform list");

    mTransforms = static_cast<TransformIfc**>(std::calloc(mCount, sizeof(TransformIfc*)));
    AssertExit((mTransforms != nullptr), "CompositeFilter", "heap allocation failed");

    uint32_t i = 0;
    for(TransformIfc* t : transforms){
        mTransforms[i++] = t;
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
    for(uint32_t i = 0; i < mCount; ++i){
        result = mTransforms[i]->apply(result);
    }

    return result;
}

void CompositeFilter::setInputThreshold(float epsilon){
    mInputThreshold = (epsilon >= 0.0f) ? epsilon : 0.0f;
}

float CompositeFilter::getInputThreshold() const{
    return mInputThreshold;
}

} // namespace Garbox
