// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DisplayStatusState.h"

namespace Garbox {

DisplayStatusState::DisplayStatusState(): StateAbs(StateType::DisplayStatus){
    // mCurrent and mNext default-initialized
}

float DisplayStatusState::getBrightness() const {
    return mCurrent.brightness;
}

float DisplayStatusState::nextBrightness() const {
    return mCurrent.brightness;
}

void DisplayStatusState::setBrightness(float value){
    if (mNext.brightness == value) {
        return;
    }
    mNext.brightness = value;
    markDirty();
}

void DisplayStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox