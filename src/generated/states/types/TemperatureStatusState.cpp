// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "TemperatureStatusState.h"

namespace Garbox {

TemperatureStatusState::TemperatureStatusState(): StateAbs(StateType::TemperatureStatus){
    // mCurrent and mNext default-initialized
}

bool TemperatureStatusState::getDriverEnabled() const {
    return mCurrent.driverEnabled;
}

bool TemperatureStatusState::getPowerEnabled() const {
    return mCurrent.powerEnabled;
}

bool TemperatureStatusState::getResetting() const {
    return mCurrent.resetting;
}

bool TemperatureStatusState::getHasFirstSample() const {
    return mCurrent.hasFirstSample;
}

bool TemperatureStatusState::nextDriverEnabled() const {
    return mCurrent.driverEnabled;
}

bool TemperatureStatusState::nextPowerEnabled() const {
    return mCurrent.powerEnabled;
}

bool TemperatureStatusState::nextResetting() const {
    return mCurrent.resetting;
}

bool TemperatureStatusState::nextHasFirstSample() const {
    return mCurrent.hasFirstSample;
}

bool TemperatureStatusState::lastDriverEnabled() const {
    return mLast.driverEnabled;
}

bool TemperatureStatusState::lastPowerEnabled() const {
    return mLast.powerEnabled;
}

bool TemperatureStatusState::lastResetting() const {
    return mLast.resetting;
}

bool TemperatureStatusState::lastHasFirstSample() const {
    return mLast.hasFirstSample;
}

void TemperatureStatusState::setDriverEnabled(bool value){
    if (mNext.driverEnabled == value) {
        return;
    }
    mNext.driverEnabled = value;
    markDirty();
}

void TemperatureStatusState::setPowerEnabled(bool value){
    if (mNext.powerEnabled == value) {
        return;
    }
    mNext.powerEnabled = value;
    markDirty();
}

void TemperatureStatusState::setResetting(bool value){
    if (mNext.resetting == value) {
        return;
    }
    mNext.resetting = value;
    markDirty();
}

void TemperatureStatusState::setHasFirstSample(bool value){
    if (mNext.hasFirstSample == value) {
        return;
    }
    mNext.hasFirstSample = value;
    markDirty();
}

void TemperatureStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mLast = mCurrent;
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox