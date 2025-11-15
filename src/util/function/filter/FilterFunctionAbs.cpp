#include "FilterFunctionAbs.h"

namespace Garbox {

FilterFunctionAbs::FilterFunctionAbs(){
    // nothing to do
}

void FilterFunctionAbs::reset(float value){
    mLastRaw = value;
    mLastFiltered = value;
    onReset(value);
}

float FilterFunctionAbs::process(float value){
    mLastFiltered = onProcess(value);
    mLastRaw = value;
    return mLastFiltered;
}

float FilterFunctionAbs::getFilteredValue() const {
    return mLastFiltered;
}

float FilterFunctionAbs::getRawValue() const {
    return mLastRaw;
}

void FilterFunctionAbs::onReset(float value){
    // nothing to do
}

} // namespace Garbox
