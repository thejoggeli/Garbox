#include "FilterFunctionAbs.h"

#include <cmath>

namespace Garbox {

FilterFunctionAbs::FilterFunctionAbs(){
    // nothing to do
}

void FilterFunctionAbs::reset(float value){
    const bool finite = std::isfinite(value);
    AssertExit(finite, "FilterFunctionAbs", "reset value must be finite");
    if(!finite){
        return;
    }
    mLastRaw = value;
    mLastFiltered = value;
    onReset(value);
}

float FilterFunctionAbs::process(float value){
    const bool finite = std::isfinite(value);
    AssertExit(finite, "FilterFunctionAbs", "input value must be finite");
    if(!finite){
        return mLastFiltered;
    }
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
