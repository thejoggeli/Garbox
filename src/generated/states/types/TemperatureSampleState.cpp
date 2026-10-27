// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "TemperatureSampleState.h"

namespace Garbox {

TemperatureSampleState::TemperatureSampleState(): StateAbs(StateType::TemperatureSample){
    // mCurrent and mNext default-initialized
}

float TemperatureSampleState::getTemperatureCelcius() const {
    return mCurrent.temperatureCelcius;
}

float TemperatureSampleState::getHumidityRelative() const {
    return mCurrent.humidityRelative;
}

uint64_t TemperatureSampleState::getSampleMicros() const {
    return mCurrent.sampleMicros;
}

float TemperatureSampleState::nextTemperatureCelcius() const {
    return mNext.temperatureCelcius;
}

float TemperatureSampleState::nextHumidityRelative() const {
    return mNext.humidityRelative;
}

uint64_t TemperatureSampleState::nextSampleMicros() const {
    return mNext.sampleMicros;
}

float TemperatureSampleState::lastTemperatureCelcius() const {
    return mLast.temperatureCelcius;
}

float TemperatureSampleState::lastHumidityRelative() const {
    return mLast.humidityRelative;
}

uint64_t TemperatureSampleState::lastSampleMicros() const {
    return mLast.sampleMicros;
}

void TemperatureSampleState::setTemperatureCelcius(float value){
    if (mNext.temperatureCelcius == value) {
        return;
    }
    mNext.temperatureCelcius = value;
    markDirty();
}

void TemperatureSampleState::setHumidityRelative(float value){
    if (mNext.humidityRelative == value) {
        return;
    }
    mNext.humidityRelative = value;
    markDirty();
}

void TemperatureSampleState::setSampleMicros(uint64_t value){
    if (mNext.sampleMicros == value) {
        return;
    }
    mNext.sampleMicros = value;
    markDirty();
}

void TemperatureSampleState::publish(){
    if (!isDirty()) {
        return;
    }
    mLast = mCurrent;
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox