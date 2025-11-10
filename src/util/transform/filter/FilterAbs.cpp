#include "FilterAbs.h"

namespace Garbox {

FilterAbs::FilterAbs(){
    // nothing to do
}

void FilterAbs::reset(float value){
    mLastRaw = value;
    mLastFiltered = value;
    onReset(value);
}

float FilterAbs::process(float value){
    mLastRaw = value;
    mLastFiltered = onProcess(value);
    return mLastFiltered;
}

float FilterAbs::getFilteredValue() const{
    return mLastFiltered;
}

float FilterAbs::getRawValue() const{
    return mLastRaw;
}

void FilterAbs::onReset(float value){
    // nothing to do
}

} // namespace Garbox
