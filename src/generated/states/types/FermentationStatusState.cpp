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

SafetyFault FermentationStatusState::getSafetyFault() const {
    return mCurrent.safetyFault;
}

float FermentationStatusState::getTargetTemperature() const {
    return mCurrent.targetTemperature;
}

bool FermentationStatusState::getFanAuto() const {
    return mCurrent.fanAuto;
}

float FermentationStatusState::getFanManualSpeed() const {
    return mCurrent.fanManualSpeed;
}

bool FermentationStatusState::getPowerOn() const {
    return mCurrent.powerOn;
}

FermentationState FermentationStatusState::nextState() const {
    return mNext.state;
}

SafetyFault FermentationStatusState::nextSafetyFault() const {
    return mNext.safetyFault;
}

float FermentationStatusState::nextTargetTemperature() const {
    return mNext.targetTemperature;
}

bool FermentationStatusState::nextFanAuto() const {
    return mNext.fanAuto;
}

float FermentationStatusState::nextFanManualSpeed() const {
    return mNext.fanManualSpeed;
}

bool FermentationStatusState::nextPowerOn() const {
    return mNext.powerOn;
}

FermentationState FermentationStatusState::lastState() const {
    return mLast.state;
}

SafetyFault FermentationStatusState::lastSafetyFault() const {
    return mLast.safetyFault;
}

float FermentationStatusState::lastTargetTemperature() const {
    return mLast.targetTemperature;
}

bool FermentationStatusState::lastFanAuto() const {
    return mLast.fanAuto;
}

float FermentationStatusState::lastFanManualSpeed() const {
    return mLast.fanManualSpeed;
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

void FermentationStatusState::setSafetyFault(SafetyFault value){
    if (mNext.safetyFault == value) {
        return;
    }
    mNext.safetyFault = value;
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

void FermentationStatusState::setFanManualSpeed(float value){
    if (mNext.fanManualSpeed == value) {
        return;
    }
    mNext.fanManualSpeed = value;
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