// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FanStatusState.h"

namespace Garbox {

FanStatusState::FanStatusState(): StateAbs(StateType::FanStatus){
    // mCurrent and mNext default-initialized
}

FanState FanStatusState::getState() const {
    return mCurrent.state;
}

float FanStatusState::getTargetSpeed() const {
    return mCurrent.targetSpeed;
}

bool FanStatusState::isRpmControl() const {
    return mCurrent.rpmControl;
}

void FanStatusState::setState(FanState value){
    if (mNext.state == value) {
        return;
    }
    mNext.state = value;
    setDirty();
}

void FanStatusState::setTargetSpeed(float value){
    if (mNext.targetSpeed == value) {
        return;
    }
    mNext.targetSpeed = value;
    setDirty();
}

void FanStatusState::setRpmControl(bool value){
    if (mNext.rpmControl == value) {
        return;
    }
    mNext.rpmControl = value;
    setDirty();
}

void FanStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
}

} // namespace Garbox