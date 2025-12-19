// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadStatusState.h"

namespace Garbox {

HeatpadStatusState::HeatpadStatusState(): StateAbs(StateType::HeatpadStatus){
    // mCurrent and mNext default-initialized
}

HeatpadState HeatpadStatusState::getState() const {
    return mCurrent.state;
}

float HeatpadStatusState::getCurrentDutyCycle() const {
    return mCurrent.currentDutyCycle;
}

uint32_t HeatpadStatusState::getCurrentPeriodMicros() const {
    return mCurrent.currentPeriodMicros;
}

float HeatpadStatusState::getNextDutyCycle() const {
    return mCurrent.nextDutyCycle;
}

uint32_t HeatpadStatusState::getNextPeriodMicros() const {
    return mCurrent.nextPeriodMicros;
}

HeatpadState HeatpadStatusState::nextState() const {
    return mCurrent.state;
}

float HeatpadStatusState::nextCurrentDutyCycle() const {
    return mCurrent.currentDutyCycle;
}

uint32_t HeatpadStatusState::nextCurrentPeriodMicros() const {
    return mCurrent.currentPeriodMicros;
}

float HeatpadStatusState::nextNextDutyCycle() const {
    return mCurrent.nextDutyCycle;
}

uint32_t HeatpadStatusState::nextNextPeriodMicros() const {
    return mCurrent.nextPeriodMicros;
}

HeatpadState HeatpadStatusState::lastState() const {
    return mLast.state;
}

float HeatpadStatusState::lastCurrentDutyCycle() const {
    return mLast.currentDutyCycle;
}

uint32_t HeatpadStatusState::lastCurrentPeriodMicros() const {
    return mLast.currentPeriodMicros;
}

float HeatpadStatusState::lastNextDutyCycle() const {
    return mLast.nextDutyCycle;
}

uint32_t HeatpadStatusState::lastNextPeriodMicros() const {
    return mLast.nextPeriodMicros;
}

void HeatpadStatusState::setState(HeatpadState value){
    if (mNext.state == value) {
        return;
    }
    mNext.state = value;
    markDirty();
}

void HeatpadStatusState::setCurrentDutyCycle(float value){
    if (mNext.currentDutyCycle == value) {
        return;
    }
    mNext.currentDutyCycle = value;
    markDirty();
}

void HeatpadStatusState::setCurrentPeriodMicros(uint32_t value){
    if (mNext.currentPeriodMicros == value) {
        return;
    }
    mNext.currentPeriodMicros = value;
    markDirty();
}

void HeatpadStatusState::setNextDutyCycle(float value){
    if (mNext.nextDutyCycle == value) {
        return;
    }
    mNext.nextDutyCycle = value;
    markDirty();
}

void HeatpadStatusState::setNextPeriodMicros(uint32_t value){
    if (mNext.nextPeriodMicros == value) {
        return;
    }
    mNext.nextPeriodMicros = value;
    markDirty();
}

void HeatpadStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mLast = mCurrent;
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox