// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "TemperatureStatusState.h"

namespace Garbox {

TemperatureStatusState::TemperatureStatusState(): StateAbs(StateType::TemperatureStatus){
    // mCurrent and mNext default-initialized
}

bool TemperatureStatusState::isDriverEnabled() const {
    return mCurrent.driverEnabled;
}

bool TemperatureStatusState::isPowerEnabled() const {
    return mCurrent.powerEnabled;
}

bool TemperatureStatusState::isResetting() const {
    return mCurrent.resetting;
}

bool TemperatureStatusState::isHasFirstSample() const {
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
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox