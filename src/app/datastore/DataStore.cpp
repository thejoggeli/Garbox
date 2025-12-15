// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DataStore.h"

namespace Garbox {

DataStore::DataStore():
    DataStoreAbs(),
    mTempsenseState(),
    mTempsenseDriverEnabled(),
    mTempsensePowerEnabled(),
    mTempsenseResetting(),
    mTempsenseHasFirstSample(),
    mMeasuredTemperature(),
    mMeasuredHumidity(),
    mDisplayBrightness(),
    mFramesSkipped(),
    mHeaterEngineState(),
    mTargetTemperature(),
    mFanState(),
    mFanTargetSpeed(),
    mFanRpmControl(),
    mFanMeasuredRpm(),
    mHeatpadState(),
    mHeatpadDuty(),
    mHeatpadPeriod(),
    mHeatpadNextDuty(),
    mHeatpadNextPeriod(),
    mHeatpadMeasuredVoltage(),
    mHeatpadMeasuredCurrent(),
    mHeatpadPwmProgress(){}

DataStore::~DataStore(){
    // nothing to do
}

void DataStore::Init(){
    instance();
}

DataStore& DataStore::instance(){
    static DataStore instance;
    return instance;
}

const StoredTempsenseState& DataStore::GetTempsenseState(){
    return instance().mTempsenseState;
}

bool DataStore::ResetTempsenseState(){
    return instance().mTempsenseState.reset();
}

bool DataStore::UpdateTempsenseState(TempsenseState value){
    return instance().mTempsenseState.update(value);
}

const StoredBool& DataStore::GetTempsenseDriverEnabled(){
    return instance().mTempsenseDriverEnabled;
}

bool DataStore::ResetTempsenseDriverEnabled(){
    return instance().mTempsenseDriverEnabled.reset();
}

bool DataStore::UpdateTempsenseDriverEnabled(bool value){
    return instance().mTempsenseDriverEnabled.update(value);
}

const StoredBool& DataStore::GetTempsensePowerEnabled(){
    return instance().mTempsensePowerEnabled;
}

bool DataStore::ResetTempsensePowerEnabled(){
    return instance().mTempsensePowerEnabled.reset();
}

bool DataStore::UpdateTempsensePowerEnabled(bool value){
    return instance().mTempsensePowerEnabled.update(value);
}

const StoredBool& DataStore::GetTempsenseResetting(){
    return instance().mTempsenseResetting;
}

bool DataStore::ResetTempsenseResetting(){
    return instance().mTempsenseResetting.reset();
}

bool DataStore::UpdateTempsenseResetting(bool value){
    return instance().mTempsenseResetting.update(value);
}

const StoredBool& DataStore::GetTempsenseHasFirstSample(){
    return instance().mTempsenseHasFirstSample;
}

bool DataStore::ResetTempsenseHasFirstSample(){
    return instance().mTempsenseHasFirstSample.reset();
}

bool DataStore::UpdateTempsenseHasFirstSample(bool value){
    return instance().mTempsenseHasFirstSample.update(value);
}

const StoredFloat& DataStore::GetMeasuredTemperature(){
    return instance().mMeasuredTemperature;
}

bool DataStore::ResetMeasuredTemperature(){
    return instance().mMeasuredTemperature.reset();
}

bool DataStore::UpdateMeasuredTemperature(float value){
    return instance().mMeasuredTemperature.update(value);
}

const StoredFloat& DataStore::GetMeasuredHumidity(){
    return instance().mMeasuredHumidity;
}

bool DataStore::ResetMeasuredHumidity(){
    return instance().mMeasuredHumidity.reset();
}

bool DataStore::UpdateMeasuredHumidity(float value){
    return instance().mMeasuredHumidity.update(value);
}

const StoredFloat& DataStore::GetDisplayBrightness(){
    return instance().mDisplayBrightness;
}

bool DataStore::ResetDisplayBrightness(){
    return instance().mDisplayBrightness.reset();
}

bool DataStore::UpdateDisplayBrightness(float value){
    return instance().mDisplayBrightness.update(value);
}

const StoredUint32& DataStore::GetFramesSkipped(){
    return instance().mFramesSkipped;
}

bool DataStore::ResetFramesSkipped(){
    return instance().mFramesSkipped.reset();
}

bool DataStore::UpdateFramesSkipped(uint32_t value){
    return instance().mFramesSkipped.update(value);
}

const StoredHeaterEngineState& DataStore::GetHeaterEngineState(){
    return instance().mHeaterEngineState;
}

bool DataStore::ResetHeaterEngineState(){
    return instance().mHeaterEngineState.reset();
}

bool DataStore::UpdateHeaterEngineState(HeaterEngineState value){
    return instance().mHeaterEngineState.update(value);
}

const StoredFloat& DataStore::GetTargetTemperature(){
    return instance().mTargetTemperature;
}

bool DataStore::ResetTargetTemperature(){
    return instance().mTargetTemperature.reset();
}

bool DataStore::UpdateTargetTemperature(float value){
    return instance().mTargetTemperature.update(value);
}

const StoredFanState& DataStore::GetFanState(){
    return instance().mFanState;
}

bool DataStore::ResetFanState(){
    return instance().mFanState.reset();
}

bool DataStore::UpdateFanState(FanState value){
    return instance().mFanState.update(value);
}

const StoredFloat& DataStore::GetFanTargetSpeed(){
    return instance().mFanTargetSpeed;
}

bool DataStore::ResetFanTargetSpeed(){
    return instance().mFanTargetSpeed.reset();
}

bool DataStore::UpdateFanTargetSpeed(float value){
    return instance().mFanTargetSpeed.update(value);
}

const StoredBool& DataStore::GetFanRpmControl(){
    return instance().mFanRpmControl;
}

bool DataStore::ResetFanRpmControl(){
    return instance().mFanRpmControl.reset();
}

bool DataStore::UpdateFanRpmControl(bool value){
    return instance().mFanRpmControl.update(value);
}

const StoredFloat& DataStore::GetFanMeasuredRpm(){
    return instance().mFanMeasuredRpm;
}

bool DataStore::ResetFanMeasuredRpm(){
    return instance().mFanMeasuredRpm.reset();
}

bool DataStore::UpdateFanMeasuredRpm(float value){
    return instance().mFanMeasuredRpm.update(value);
}

const StoredHeatpadState& DataStore::GetHeatpadState(){
    return instance().mHeatpadState;
}

bool DataStore::ResetHeatpadState(){
    return instance().mHeatpadState.reset();
}

bool DataStore::UpdateHeatpadState(HeatpadState value){
    return instance().mHeatpadState.update(value);
}

const StoredFloat& DataStore::GetHeatpadDuty(){
    return instance().mHeatpadDuty;
}

bool DataStore::ResetHeatpadDuty(){
    return instance().mHeatpadDuty.reset();
}

bool DataStore::UpdateHeatpadDuty(float value){
    return instance().mHeatpadDuty.update(value);
}

const StoredUint32& DataStore::GetHeatpadPeriod(){
    return instance().mHeatpadPeriod;
}

bool DataStore::ResetHeatpadPeriod(){
    return instance().mHeatpadPeriod.reset();
}

bool DataStore::UpdateHeatpadPeriod(uint32_t value){
    return instance().mHeatpadPeriod.update(value);
}

const StoredFloat& DataStore::GetHeatpadNextDuty(){
    return instance().mHeatpadNextDuty;
}

bool DataStore::ResetHeatpadNextDuty(){
    return instance().mHeatpadNextDuty.reset();
}

bool DataStore::UpdateHeatpadNextDuty(float value){
    return instance().mHeatpadNextDuty.update(value);
}

const StoredUint32& DataStore::GetHeatpadNextPeriod(){
    return instance().mHeatpadNextPeriod;
}

bool DataStore::ResetHeatpadNextPeriod(){
    return instance().mHeatpadNextPeriod.reset();
}

bool DataStore::UpdateHeatpadNextPeriod(uint32_t value){
    return instance().mHeatpadNextPeriod.update(value);
}

const StoredFloat& DataStore::GetHeatpadMeasuredVoltage(){
    return instance().mHeatpadMeasuredVoltage;
}

bool DataStore::ResetHeatpadMeasuredVoltage(){
    return instance().mHeatpadMeasuredVoltage.reset();
}

bool DataStore::UpdateHeatpadMeasuredVoltage(float value){
    return instance().mHeatpadMeasuredVoltage.update(value);
}

const StoredFloat& DataStore::GetHeatpadMeasuredCurrent(){
    return instance().mHeatpadMeasuredCurrent;
}

bool DataStore::ResetHeatpadMeasuredCurrent(){
    return instance().mHeatpadMeasuredCurrent.reset();
}

bool DataStore::UpdateHeatpadMeasuredCurrent(float value){
    return instance().mHeatpadMeasuredCurrent.update(value);
}

const StoredUint32& DataStore::GetHeatpadPwmProgress(){
    return instance().mHeatpadPwmProgress;
}

bool DataStore::ResetHeatpadPwmProgress(){
    return instance().mHeatpadPwmProgress.reset();
}

bool DataStore::UpdateHeatpadPwmProgress(uint32_t value){
    return instance().mHeatpadPwmProgress.update(value);
}

} // namespace