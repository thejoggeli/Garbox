// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"
#include <math.h>
#include "shared/types/ComponentId.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs(): 
    ScreenAbs(
        ComponentId::MainScreen, 
        ScreenId::Main, 
        static_cast<uint32_t>(Model::Index::Count)
    ),
    mModel(*this){
    // nothing to do
}

void MainScreenAbs::init(ComponentHostIfc& host){
    ScreenAbs::init(host);
    registerUpdateHandler(applyFanStateTrampoline, this);
    registerUpdateHandler(applyFanMeasuredRpmTrampoline, this);
    registerUpdateHandler(applyHeatpadStateTrampoline, this);
    registerUpdateHandler(applyHeatpadDutyTrampoline, this);
    registerUpdateHandler(applyBoxPositionTrampoline, this);
    registerUpdateHandler(applyHeatpadSenseTrampoline, this);
    registerUpdateHandler(applyDisplayStatusTrampoline, this);
    registerUpdateHandler(applyTemperatureStateTrampoline, this);
    registerUpdateHandler(applyTemperatureSampleTrampoline, this);
    registerUpdateHandler(applyAppInfoTrampoline, this);
    registerUpdateHandler(applyFermentationStatusTrampoline, this);
    registerUpdateHandler(applyHeapSpaceTrampoline, this);
}

MainScreenAbs::Model& MainScreenAbs::getModel(){
    return mModel;
}

const MainScreenAbs::Model& MainScreenAbs::getModel() const {
    return mModel;
}

void MainScreenAbs::markDirty(Model::Index index){
    markDirtyRaw(static_cast<size_t>(index));
}

bool MainScreenAbs::isMarkedDirty(Model::Index index) const {
    return isMarkedDirtyRaw(static_cast<size_t>(index));
}

DisplayCommandEvent MainScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void MainScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

void MainScreenAbs::applyFanStateTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyFanState();
}

void MainScreenAbs::applyFanMeasuredRpmTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyFanMeasuredRpm();
}

void MainScreenAbs::applyHeatpadStateTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyHeatpadState();
}

void MainScreenAbs::applyHeatpadDutyTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyHeatpadDuty();
}

void MainScreenAbs::applyBoxPositionTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyBoxPosition();
}

void MainScreenAbs::applyHeatpadSenseTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyHeatpadSense();
}

void MainScreenAbs::applyDisplayStatusTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyDisplayStatus();
}

void MainScreenAbs::applyTemperatureStateTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyTemperatureState();
}

void MainScreenAbs::applyTemperatureSampleTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyTemperatureSample();
}

void MainScreenAbs::applyAppInfoTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyAppInfo();
}

void MainScreenAbs::applyFermentationStatusTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyFermentationStatus();
}

void MainScreenAbs::applyHeapSpaceTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyHeapSpace();
}

MainScreenAbs::Model::Model(MainScreenAbs& screen) : mScreen(screen){ 
    // nothing to do 
}

FanState MainScreenAbs::Model::getFanState() const { 
    return mFanState; 
}

float MainScreenAbs::Model::getFanTargetSpeed() const { 
    return mFanTargetSpeed; 
}

float MainScreenAbs::Model::getFanMeasuredRpm() const { 
    return mFanMeasuredRpm; 
}

HeatpadState MainScreenAbs::Model::getHeatpadState() const { 
    return mHeatpadState; 
}

float MainScreenAbs::Model::getHeatpadCurrentDuty() const { 
    return mHeatpadCurrentDuty; 
}

uint32_t MainScreenAbs::Model::getHeatpadCurrentPeriod() const { 
    return mHeatpadCurrentPeriod; 
}

float MainScreenAbs::Model::getHeatpadNextDuty() const { 
    return mHeatpadNextDuty; 
}

uint32_t MainScreenAbs::Model::getHeatpadNextPeriod() const { 
    return mHeatpadNextPeriod; 
}

float MainScreenAbs::Model::getHeatpadPwmProgress() const { 
    return mHeatpadPwmProgress; 
}

float MainScreenAbs::Model::getHeatpadMeasuredVoltage() const { 
    return mHeatpadMeasuredVoltage; 
}

float MainScreenAbs::Model::getHeatpadMeasuredCurrent() const { 
    return mHeatpadMeasuredCurrent; 
}

float MainScreenAbs::Model::getDisplayBrightness() const { 
    return mDisplayBrightness; 
}

uint32_t MainScreenAbs::Model::getDisplaySkipped() const { 
    return mDisplaySkipped; 
}

bool MainScreenAbs::Model::getShtDriverEnabled() const { 
    return mShtDriverEnabled; 
}

bool MainScreenAbs::Model::getShtPowerEnabled() const { 
    return mShtPowerEnabled; 
}

bool MainScreenAbs::Model::getShtResetting() const { 
    return mShtResetting; 
}

float MainScreenAbs::Model::getSensorTemperatureCelcius() const { 
    return mSensorTemperatureCelcius; 
}

float MainScreenAbs::Model::getSensorHumidityRelative() const { 
    return mSensorHumidityRelative; 
}

uint32_t MainScreenAbs::Model::getEventCount() const { 
    return mEventCount; 
}

BehaviourId MainScreenAbs::Model::getBehaviour() const { 
    return mBehaviour; 
}

HeaterEngineState MainScreenAbs::Model::getEngineState() const { 
    return mEngineState; 
}

float MainScreenAbs::Model::getEngineTargetTemperature() const { 
    return mEngineTargetTemperature; 
}

float MainScreenAbs::Model::getEngineMeasuredTemperature() const { 
    return mEngineMeasuredTemperature; 
}

float MainScreenAbs::Model::getEngineMeasuredHumidity() const { 
    return mEngineMeasuredHumidity; 
}

uint32_t MainScreenAbs::Model::getHeapSpace() const { 
    return mHeapSpace; 
}

void MainScreenAbs::Model::setFanState(FanState value){ 
    if(mFanState != value) { 
        mFanState = value; 
        mScreen.markDirty(Model::Index::FanState);
    } 
}

void MainScreenAbs::Model::setFanTargetSpeed(float value){ 
    if(mFanTargetSpeed != value) { 
        mFanTargetSpeed = value; 
        mScreen.markDirty(Model::Index::FanState);
    } 
}

void MainScreenAbs::Model::setFanMeasuredRpm(float value){ 
    if(mFanMeasuredRpm != value) { 
        mFanMeasuredRpm = value; 
        mScreen.markDirty(Model::Index::FanMeasuredRpm);
    } 
}

void MainScreenAbs::Model::setHeatpadState(HeatpadState value){ 
    if(mHeatpadState != value) { 
        mHeatpadState = value; 
        mScreen.markDirty(Model::Index::HeatpadState);
    } 
}

void MainScreenAbs::Model::setHeatpadCurrentDuty(float value){ 
    if(mHeatpadCurrentDuty != value) { 
        mHeatpadCurrentDuty = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void MainScreenAbs::Model::setHeatpadCurrentPeriod(uint32_t value){ 
    if(mHeatpadCurrentPeriod != value) { 
        mHeatpadCurrentPeriod = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void MainScreenAbs::Model::setHeatpadNextDuty(float value){ 
    if(mHeatpadNextDuty != value) { 
        mHeatpadNextDuty = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void MainScreenAbs::Model::setHeatpadNextPeriod(uint32_t value){ 
    if(mHeatpadNextPeriod != value) { 
        mHeatpadNextPeriod = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void MainScreenAbs::Model::setHeatpadPwmProgress(float value){ 
    if(mHeatpadPwmProgress != value) { 
        mHeatpadPwmProgress = value; 
        mScreen.markDirty(Model::Index::BoxPosition);
    } 
}

void MainScreenAbs::Model::setHeatpadMeasuredVoltage(float value){ 
    if(mHeatpadMeasuredVoltage != value) { 
        mHeatpadMeasuredVoltage = value; 
        mScreen.markDirty(Model::Index::HeatpadSense);
    } 
}

void MainScreenAbs::Model::setHeatpadMeasuredCurrent(float value){ 
    if(mHeatpadMeasuredCurrent != value) { 
        mHeatpadMeasuredCurrent = value; 
        mScreen.markDirty(Model::Index::HeatpadSense);
    } 
}

void MainScreenAbs::Model::setDisplayBrightness(float value){ 
    if(mDisplayBrightness != value) { 
        mDisplayBrightness = value; 
        mScreen.markDirty(Model::Index::DisplayStatus);
    } 
}

void MainScreenAbs::Model::setDisplaySkipped(uint32_t value){ 
    if(mDisplaySkipped != value) { 
        mDisplaySkipped = value; 
        mScreen.markDirty(Model::Index::DisplayStatus);
    } 
}

void MainScreenAbs::Model::setShtDriverEnabled(bool value){ 
    if(mShtDriverEnabled != value) { 
        mShtDriverEnabled = value; 
        mScreen.markDirty(Model::Index::TemperatureState);
    } 
}

void MainScreenAbs::Model::setShtPowerEnabled(bool value){ 
    if(mShtPowerEnabled != value) { 
        mShtPowerEnabled = value; 
        mScreen.markDirty(Model::Index::TemperatureState);
    } 
}

void MainScreenAbs::Model::setShtResetting(bool value){ 
    if(mShtResetting != value) { 
        mShtResetting = value; 
        mScreen.markDirty(Model::Index::TemperatureState);
    } 
}

void MainScreenAbs::Model::setSensorTemperatureCelcius(float value){ 
    if(mSensorTemperatureCelcius != value) { 
        mSensorTemperatureCelcius = value; 
        mScreen.markDirty(Model::Index::TemperatureSample);
    } 
}

void MainScreenAbs::Model::setSensorHumidityRelative(float value){ 
    if(mSensorHumidityRelative != value) { 
        mSensorHumidityRelative = value; 
        mScreen.markDirty(Model::Index::TemperatureSample);
    } 
}

void MainScreenAbs::Model::setEventCount(uint32_t value){ 
    if(mEventCount != value) { 
        mEventCount = value; 
        mScreen.markDirty(Model::Index::AppInfo);
    } 
}

void MainScreenAbs::Model::setBehaviour(BehaviourId value){ 
    if(mBehaviour != value) { 
        mBehaviour = value; 
        mScreen.markDirty(Model::Index::AppInfo);
    } 
}

void MainScreenAbs::Model::setEngineState(HeaterEngineState value){ 
    if(mEngineState != value) { 
        mEngineState = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void MainScreenAbs::Model::setEngineTargetTemperature(float value){ 
    if(mEngineTargetTemperature != value) { 
        mEngineTargetTemperature = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void MainScreenAbs::Model::setEngineMeasuredTemperature(float value){ 
    if(mEngineMeasuredTemperature != value) { 
        mEngineMeasuredTemperature = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void MainScreenAbs::Model::setEngineMeasuredHumidity(float value){ 
    if(mEngineMeasuredHumidity != value) { 
        mEngineMeasuredHumidity = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void MainScreenAbs::Model::setHeapSpace(uint32_t value){ 
    if(mHeapSpace != value) { 
        mHeapSpace = value; 
        mScreen.markDirty(Model::Index::HeapSpace);
    } 
}

} // namespace Garbox