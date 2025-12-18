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

void TemperatureSampleState::setTemperatureCelcius(float value){
    if (mNext.temperatureCelcius == value) {
        return;
    }
    mNext.temperatureCelcius = value;
    setDirty();
}

void TemperatureSampleState::setHumidityRelative(float value){
    if (mNext.humidityRelative == value) {
        return;
    }
    mNext.humidityRelative = value;
    setDirty();
}

void TemperatureSampleState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
}

} // namespace Garbox