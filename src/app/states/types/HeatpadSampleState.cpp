// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadSampleState.h"

namespace Garbox {

HeatpadSampleState::HeatpadSampleState(): StateAbs(StateType::HeatpadSample){
    // mCurrent and mNext default-initialized
}

float HeatpadSampleState::getMeasuredVoltage() const {
    return mCurrent.measuredVoltage;
}

float HeatpadSampleState::getMeasuredCurrent() const {
    return mCurrent.measuredCurrent;
}

float HeatpadSampleState::getPwmProgressMicros() const {
    return mCurrent.pwmProgressMicros;
}

float HeatpadSampleState::nextMeasuredVoltage() const {
    return mCurrent.measuredVoltage;
}

float HeatpadSampleState::nextMeasuredCurrent() const {
    return mCurrent.measuredCurrent;
}

float HeatpadSampleState::nextPwmProgressMicros() const {
    return mCurrent.pwmProgressMicros;
}

void HeatpadSampleState::setMeasuredVoltage(float value){
    if (mNext.measuredVoltage == value) {
        return;
    }
    mNext.measuredVoltage = value;
    markDirty();
}

void HeatpadSampleState::setMeasuredCurrent(float value){
    if (mNext.measuredCurrent == value) {
        return;
    }
    mNext.measuredCurrent = value;
    markDirty();
}

void HeatpadSampleState::setPwmProgressMicros(float value){
    if (mNext.pwmProgressMicros == value) {
        return;
    }
    mNext.pwmProgressMicros = value;
    markDirty();
}

void HeatpadSampleState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox