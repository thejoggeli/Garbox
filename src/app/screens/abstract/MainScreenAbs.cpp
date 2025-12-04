// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"
#include <math.h>
#include "core/lvgl/LvglProvider.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs(): 
    ScreenAbs(ComponentId::MainScreen, ScreenId::Main),
    mContainer(LvglProvider::Root()),
    mDisplayWidth(LvglProvider::GetDisplayWidth()),
    mDisplayHeight(LvglProvider::GetDisplayHeight()),
    mModel(*this),
    mDirtyDispatcher(static_cast<uint32_t>(Model::Index::Count)){}

void MainScreenAbs::init(ComponentHostIfc& host){
    ScreenAbs::init(host);

    // init lvgl container
    mContainer.setHidden(true);
    mContainer.setSize(mDisplayWidth, mDisplayHeight);
    mContainer.setBorder(0, lv_color_hex(0x000000));
    mContainer.setRadius(0);
    mContainer.setPad(0, 0, 0, 0);
    mContainer.setBgOpacity(LV_OPA_COVER);
    mContainer.setScrollable(false);
    mContainer.clearFlag(LV_OBJ_FLAG_SCROLL_CHAIN);
    mContainer.clearFlag(LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    mDirtyDispatcher.registerHandler(applyFanStatusTrampoline, this);
    mDirtyDispatcher.registerHandler(applyFanTargetSpeedTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadStatusTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadMeasureTrampoline, this);
    mDirtyDispatcher.registerHandler(applyDisplayBrightnessTrampoline, this);
    mDirtyDispatcher.registerHandler(applySensorStatusTrampoline, this);
    mDirtyDispatcher.registerHandler(applyMeasuredTemperatureTrampoline, this);
    mDirtyDispatcher.registerHandler(applyMeasuredHumidityTrampoline, this);
    mDirtyDispatcher.registerHandler(applyTargetTemperatureTrampoline, this);
    mDirtyDispatcher.registerHandler(applyEngineStateTrampoline, this);
}

void MainScreenAbs::updateScreen(){
    ScreenAbs::updateScreen();
    mDispatchedCount += mDirtyDispatcher.getDirtyCount();
    mDirtyDispatcher.dispatch();
}

void MainScreenAbs::becomeEnabled(){
    mContainer.setHidden(false);
    ScreenAbs::becomeEnabled();
}

void MainScreenAbs::becomeDisabled(){
    mContainer.setHidden(true);
    ScreenAbs::becomeDisabled();
}

void MainScreenAbs::setBackgroundColor(uint32_t color){
    mContainer.setBgColor(lv_color_hex(color));
}

MainScreenAbs::Model::Model(MainScreenAbs& screen) : mScreen(screen){ 
    // nothing to do 
}

FanState MainScreenAbs::Model::getFanState() const { 
    return mFanState; 
}

float MainScreenAbs::Model::getFanMeasuredRpm() const { 
    return mFanMeasuredRpm; 
}

float MainScreenAbs::Model::getFanTargetSpeed() const { 
    return mFanTargetSpeed; 
}

HeatpadState MainScreenAbs::Model::getHeatpadState() const { 
    return mHeatpadState; 
}

float MainScreenAbs::Model::getHeatpadCurrentDuty() const { 
    return mHeatpadCurrentDuty; 
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

bool MainScreenAbs::Model::getShtDriverEnabled() const { 
    return mShtDriverEnabled; 
}

bool MainScreenAbs::Model::getShtPowerEnabled() const { 
    return mShtPowerEnabled; 
}

bool MainScreenAbs::Model::getShtResetting() const { 
    return mShtResetting; 
}

bool MainScreenAbs::Model::getShtHasSample() const { 
    return mShtHasSample; 
}

float MainScreenAbs::Model::getMeasuredTemperature() const { 
    return mMeasuredTemperature; 
}

float MainScreenAbs::Model::getMeasuredHumidity() const { 
    return mMeasuredHumidity; 
}

float MainScreenAbs::Model::getTargetTemperature() const { 
    return mTargetTemperature; 
}

HeaterEngineState MainScreenAbs::Model::getEngineState() const { 
    return mEngineState; 
}

void MainScreenAbs::Model::setFanState(FanState value){ 
    if(mFanState != value) { 
        mFanState = value; 
        mScreen.markDirty(Model::Index::FanStatus);
    } 
}

void MainScreenAbs::Model::setFanMeasuredRpm(float value){ 
    if(mFanMeasuredRpm != value) { 
        mFanMeasuredRpm = value; 
        mScreen.markDirty(Model::Index::FanStatus);
    } 
}

void MainScreenAbs::Model::setFanTargetSpeed(float value){ 
    if(mFanTargetSpeed != value) { 
        mFanTargetSpeed = value; 
        mScreen.markDirty(Model::Index::FanTargetSpeed);
    } 
}

void MainScreenAbs::Model::setHeatpadState(HeatpadState value){ 
    if(mHeatpadState != value) { 
        mHeatpadState = value; 
        mScreen.markDirty(Model::Index::HeatpadStatus);
    } 
}

void MainScreenAbs::Model::setHeatpadCurrentDuty(float value){ 
    if(mHeatpadCurrentDuty != value) { 
        mHeatpadCurrentDuty = value; 
        mScreen.markDirty(Model::Index::HeatpadStatus);
        mScreen.markDirty(Model::Index::HeatpadMeasure);
    } 
}

void MainScreenAbs::Model::setHeatpadMeasuredVoltage(float value){ 
    if(mHeatpadMeasuredVoltage != value) { 
        mHeatpadMeasuredVoltage = value; 
        mScreen.markDirty(Model::Index::HeatpadMeasure);
    } 
}

void MainScreenAbs::Model::setHeatpadMeasuredCurrent(float value){ 
    if(mHeatpadMeasuredCurrent != value) { 
        mHeatpadMeasuredCurrent = value; 
        mScreen.markDirty(Model::Index::HeatpadMeasure);
    } 
}

void MainScreenAbs::Model::setDisplayBrightness(float value){ 
    if(mDisplayBrightness != value) { 
        mDisplayBrightness = value; 
        mScreen.markDirty(Model::Index::DisplayBrightness);
    } 
}

void MainScreenAbs::Model::setShtDriverEnabled(bool value){ 
    if(mShtDriverEnabled != value) { 
        mShtDriverEnabled = value; 
        mScreen.markDirty(Model::Index::SensorStatus);
        mScreen.markDirty(Model::Index::MeasuredTemperature);
        mScreen.markDirty(Model::Index::MeasuredHumidity);
    } 
}

void MainScreenAbs::Model::setShtPowerEnabled(bool value){ 
    if(mShtPowerEnabled != value) { 
        mShtPowerEnabled = value; 
        mScreen.markDirty(Model::Index::SensorStatus);
        mScreen.markDirty(Model::Index::MeasuredTemperature);
        mScreen.markDirty(Model::Index::MeasuredHumidity);
    } 
}

void MainScreenAbs::Model::setShtResetting(bool value){ 
    if(mShtResetting != value) { 
        mShtResetting = value; 
        mScreen.markDirty(Model::Index::SensorStatus);
        mScreen.markDirty(Model::Index::MeasuredTemperature);
        mScreen.markDirty(Model::Index::MeasuredHumidity);
    } 
}

void MainScreenAbs::Model::setShtHasSample(bool value){ 
    if(mShtHasSample != value) { 
        mShtHasSample = value; 
        mScreen.markDirty(Model::Index::SensorStatus);
        mScreen.markDirty(Model::Index::MeasuredTemperature);
        mScreen.markDirty(Model::Index::MeasuredHumidity);
    } 
}

void MainScreenAbs::Model::setMeasuredTemperature(float value){ 
    if(mMeasuredTemperature != value) { 
        mMeasuredTemperature = value; 
        mScreen.markDirty(Model::Index::MeasuredTemperature);
    } 
}

void MainScreenAbs::Model::setMeasuredHumidity(float value){ 
    if(mMeasuredHumidity != value) { 
        mMeasuredHumidity = value; 
        mScreen.markDirty(Model::Index::MeasuredHumidity);
    } 
}

void MainScreenAbs::Model::setTargetTemperature(float value){ 
    if(mTargetTemperature != value) { 
        mTargetTemperature = value; 
        mScreen.markDirty(Model::Index::TargetTemperature);
    } 
}

void MainScreenAbs::Model::setEngineState(HeaterEngineState value){ 
    if(mEngineState != value) { 
        mEngineState = value; 
        mScreen.markDirty(Model::Index::EngineState);
    } 
}

MainScreenAbs::Model& MainScreenAbs::getModel(){
    return mModel;
}

const MainScreenAbs::Model& MainScreenAbs::getModel() const {
    return mModel;
}

void MainScreenAbs::markDirty(Model::Index index){
    mDirtyDispatcher.markDirty(static_cast<size_t>(index));
}

bool MainScreenAbs::isMarkedDirty(Model::Index index) const {
    return mDirtyDispatcher.isMarkedDirty(static_cast<size_t>(index));
}

void MainScreenAbs::applyFanStatusTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyFanStatus();
}

void MainScreenAbs::applyFanTargetSpeedTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyFanTargetSpeed();
}

void MainScreenAbs::applyHeatpadStatusTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyHeatpadStatus();
}

void MainScreenAbs::applyHeatpadMeasureTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyHeatpadMeasure();
}

void MainScreenAbs::applyDisplayBrightnessTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyDisplayBrightness();
}

void MainScreenAbs::applySensorStatusTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplySensorStatus();
}

void MainScreenAbs::applyMeasuredTemperatureTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyMeasuredTemperature();
}

void MainScreenAbs::applyMeasuredHumidityTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyMeasuredHumidity();
}

void MainScreenAbs::applyTargetTemperatureTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyTargetTemperature();
}

void MainScreenAbs::applyEngineStateTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyEngineState();
}

} // namespace Garbox