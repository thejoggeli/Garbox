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

FanState FanStatusState::nextState() const {
    return mCurrent.state;
}

float FanStatusState::nextTargetSpeed() const {
    return mCurrent.targetSpeed;
}

bool FanStatusState::nextRpmControl() const {
    return mCurrent.rpmControl;
}

void FanStatusState::setState(FanState value){
    if (mNext.state == value) {
        return;
    }
    mNext.state = value;
    markDirty();
}

void FanStatusState::setTargetSpeed(float value){
    if (mNext.targetSpeed == value) {
        return;
    }
    mNext.targetSpeed = value;
    markDirty();
}

void FanStatusState::setRpmControl(bool value){
    if (mNext.rpmControl == value) {
        return;
    }
    mNext.rpmControl = value;
    markDirty();
}

void FanStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox