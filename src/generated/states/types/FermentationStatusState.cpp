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

FermentationState FermentationStatusState::nextState() const {
    return mCurrent.state;
}

float FermentationStatusState::nextTargetTemperature() const {
    return mCurrent.targetTemperature;
}

FermentationState FermentationStatusState::lastState() const {
    return mLast.state;
}

float FermentationStatusState::lastTargetTemperature() const {
    return mLast.targetTemperature;
}

void FermentationStatusState::setState(FermentationState value){
    if (mNext.state == value) {
        return;
    }
    mNext.state = value;
    markDirty();
}

void FermentationStatusState::setTargetTemperature(float value){
    if (mNext.targetTemperature == value) {
        return;
    }
    mNext.targetTemperature = value;
    markDirty();
}

void FermentationStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mLast = mCurrent;
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox