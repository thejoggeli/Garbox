// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FanSampleState.h"

namespace Garbox {

FanSampleState::FanSampleState(): StateAbs(StateType::FanSample){
    // mCurrent and mNext default-initialized
}

float FanSampleState::getMeasuredRpm() const {
    return mCurrent.measuredRpm;
}

float FanSampleState::nextMeasuredRpm() const {
    return mCurrent.measuredRpm;
}

void FanSampleState::setMeasuredRpm(float value){
    if (mNext.measuredRpm == value) {
        return;
    }
    mNext.measuredRpm = value;
    markDirty();
}

void FanSampleState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox