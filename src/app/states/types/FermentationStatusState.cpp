// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FermentationStatusState.h"

namespace Garbox {

FermentationStatusState::FermentationStatusState(): StateAbs(StateType::FermentationStatus){
    // mCurrent and mNext default-initialized
}

FermentationState FermentationStatusState::getState() const {
    return mCurrent.state;
}

float FermentationStatusState::getTargetTemperature() const {
    return mCurrent.targetTemperature;
}

void FermentationStatusState::setState(FermentationState value){
    if (mNext.state == value) {
        return;
    }
    mNext.state = value;
    setDirty();
}

void FermentationStatusState::setTargetTemperature(float value){
    if (mNext.targetTemperature == value) {
        return;
    }
    mNext.targetTemperature = value;
    setDirty();
}

void FermentationStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
}

} // namespace Garbox