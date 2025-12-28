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

bool FermentationStatusState::getFanAuto() const {
    return mCurrent.fanAuto;
}

bool FermentationStatusState::getPowerOn() const {
    return mCurrent.powerOn;
}

FermentationState FermentationStatusState::nextState() const {
    return mCurrent.state;
}

float FermentationStatusState::nextTargetTemperature() const {
    return mCurrent.targetTemperature;
}

bool FermentationStatusState::nextFanAuto() const {
    return mCurrent.fanAuto;
}

bool FermentationStatusState::nextPowerOn() const {
    return mCurrent.powerOn;
}

FermentationState FermentationStatusState::lastState() const {
    return mLast.state;
}

float FermentationStatusState::lastTargetTemperature() const {
    return mLast.targetTemperature;
}

bool FermentationStatusState::lastFanAuto() const {
    return mLast.fanAuto;
}

bool FermentationStatusState::lastPowerOn() const {
    return mLast.powerOn;
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

void FermentationStatusState::setFanAuto(bool value){
    if (mNext.fanAuto == value) {
        return;
    }
    mNext.fanAuto = value;
    markDirty();
}

void FermentationStatusState::setPowerOn(bool value){
    if (mNext.powerOn == value) {
        return;
    }
    mNext.powerOn = value;
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