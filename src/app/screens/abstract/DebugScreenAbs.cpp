// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"
#include <math.h>
#include "core/lvgl/LvglProvider.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs(): 
    ScreenAbs(ComponentId::DebugScreen, ScreenId::Debug),
    mRoot(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()),
    mModel(*this),
    mDirtyDispatcher(static_cast<uint32_t>(Model::Index::Count)){}

void DebugScreenAbs::init(ComponentHostIfc& host){

    // init lvgl container
    mRoot.setRawSize(mScreenWidth, mScreenHeight);
    mRoot.setBorder(0, lv_color_hex(0x000000));
    mRoot.setRadius(0);
    mRoot.setPad(0, 0, 0, 0);
    mRoot.setBgOpa(LV_OPA_COVER);
    mRoot.setScrollable(false);

    mDirtyDispatcher.registerHandler(applyFanStateTrampoline, this);
    mDirtyDispatcher.registerHandler(applyFanMeasuredRpmTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadStateTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadDutyTrampoline, this);
    mDirtyDispatcher.registerHandler(applyBoxPositionTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadSenseTrampoline, this);
    mDirtyDispatcher.registerHandler(applyDisplayStatusTrampoline, this);
    mDirtyDispatcher.registerHandler(applyTemperatureStateTrampoline, this);
    mDirtyDispatcher.registerHandler(applyTemperatureSampleTrampoline, this);
    mDirtyDispatcher.registerHandler(applyTimeTrampoline, this);
    mDirtyDispatcher.registerHandler(applyAppInfoTrampoline, this);
    mDirtyDispatcher.registerHandler(applyFermentationStatusTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeapBlocksTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeapBytesTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeapMinimumTrampoline, this);

    // calls onInit()
    ScreenAbs::init(host);
}

void DebugScreenAbs::updateScreen(){
    ScreenAbs::updateScreen();
    mDispatchedCount += mDirtyDispatcher.getDirtyCount();
    mDirtyDispatcher.dispatch();
}

void DebugScreenAbs::becomeEnabled(){
    mRoot.setScreen();
    ScreenAbs::becomeEnabled();
}

void DebugScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

DisplayCommandEvent DebugScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void DebugScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

void DebugScreenAbs::setBackgroundColor(uint32_t color){
    mRoot.setBgColor(lv_color_hex(color));
}

DebugScreenAbs::Model::Model(DebugScreenAbs& screen) : mScreen(screen){ 
    // nothing to do 
}

FanState DebugScreenAbs::Model::getFanState() const { 
    return mFanState; 
}

float DebugScreenAbs::Model::getFanTargetSpeed() const { 
    return mFanTargetSpeed; 
}

float DebugScreenAbs::Model::getFanMeasuredRpm() const { 
    return mFanMeasuredRpm; 
}

HeatpadState DebugScreenAbs::Model::getHeatpadState() const { 
    return mHeatpadState; 
}

float DebugScreenAbs::Model::getHeatpadCurrentDuty() const { 
    return mHeatpadCurrentDuty; 
}

uint32_t DebugScreenAbs::Model::getHeatpadCurrentPeriod() const { 
    return mHeatpadCurrentPeriod; 
}

float DebugScreenAbs::Model::getHeatpadNextDuty() const { 
    return mHeatpadNextDuty; 
}

uint32_t DebugScreenAbs::Model::getHeatpadNextPeriod() const { 
    return mHeatpadNextPeriod; 
}

float DebugScreenAbs::Model::getHeatpadPwmProgress() const { 
    return mHeatpadPwmProgress; 
}

float DebugScreenAbs::Model::getHeatpadMeasuredVoltage() const { 
    return mHeatpadMeasuredVoltage; 
}

float DebugScreenAbs::Model::getHeatpadMeasuredCurrent() const { 
    return mHeatpadMeasuredCurrent; 
}

float DebugScreenAbs::Model::getDisplayBrightness() const { 
    return mDisplayBrightness; 
}

uint32_t DebugScreenAbs::Model::getDisplaySkipped() const { 
    return mDisplaySkipped; 
}

bool DebugScreenAbs::Model::getShtDriverEnabled() const { 
    return mShtDriverEnabled; 
}

bool DebugScreenAbs::Model::getShtPowerEnabled() const { 
    return mShtPowerEnabled; 
}

bool DebugScreenAbs::Model::getShtResetting() const { 
    return mShtResetting; 
}

float DebugScreenAbs::Model::getSensorTemperatureCelcius() const { 
    return mSensorTemperatureCelcius; 
}

float DebugScreenAbs::Model::getSensorHumidityRelative() const { 
    return mSensorHumidityRelative; 
}

uint32_t DebugScreenAbs::Model::getTimeSeconds() const { 
    return mTimeSeconds; 
}

uint32_t DebugScreenAbs::Model::getEventCount() const { 
    return mEventCount; 
}

BehaviourId DebugScreenAbs::Model::getBehaviour() const { 
    return mBehaviour; 
}

HeaterEngineState DebugScreenAbs::Model::getEngineState() const { 
    return mEngineState; 
}

float DebugScreenAbs::Model::getEngineTargetTemperature() const { 
    return mEngineTargetTemperature; 
}

float DebugScreenAbs::Model::getEngineMeasuredTemperature() const { 
    return mEngineMeasuredTemperature; 
}

float DebugScreenAbs::Model::getEngineMeasuredHumidity() const { 
    return mEngineMeasuredHumidity; 
}

uint32_t DebugScreenAbs::Model::getHeapAllocatedBlocks() const { 
    return mHeapAllocatedBlocks; 
}

uint32_t DebugScreenAbs::Model::getHeapFreeBlocks() const { 
    return mHeapFreeBlocks; 
}

uint32_t DebugScreenAbs::Model::getHeapLargestFreeBlock() const { 
    return mHeapLargestFreeBlock; 
}

uint32_t DebugScreenAbs::Model::getHeapAllocatedBytes() const { 
    return mHeapAllocatedBytes; 
}

uint32_t DebugScreenAbs::Model::getHeapMinimumFreeBytes() const { 
    return mHeapMinimumFreeBytes; 
}

uint32_t DebugScreenAbs::Model::getHeapTotalFreeBytes() const { 
    return mHeapTotalFreeBytes; 
}

uint32_t DebugScreenAbs::Model::getHeapMinimumTime() const { 
    return mHeapMinimumTime; 
}

void DebugScreenAbs::Model::setFanState(FanState value){ 
    if(mFanState != value) { 
        mFanState = value; 
        mScreen.markDirty(Model::Index::FanState);
    } 
}

void DebugScreenAbs::Model::setFanTargetSpeed(float value){ 
    if(mFanTargetSpeed != value) { 
        mFanTargetSpeed = value; 
        mScreen.markDirty(Model::Index::FanState);
    } 
}

void DebugScreenAbs::Model::setFanMeasuredRpm(float value){ 
    if(mFanMeasuredRpm != value) { 
        mFanMeasuredRpm = value; 
        mScreen.markDirty(Model::Index::FanMeasuredRpm);
    } 
}

void DebugScreenAbs::Model::setHeatpadState(HeatpadState value){ 
    if(mHeatpadState != value) { 
        mHeatpadState = value; 
        mScreen.markDirty(Model::Index::HeatpadState);
    } 
}

void DebugScreenAbs::Model::setHeatpadCurrentDuty(float value){ 
    if(mHeatpadCurrentDuty != value) { 
        mHeatpadCurrentDuty = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void DebugScreenAbs::Model::setHeatpadCurrentPeriod(uint32_t value){ 
    if(mHeatpadCurrentPeriod != value) { 
        mHeatpadCurrentPeriod = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void DebugScreenAbs::Model::setHeatpadNextDuty(float value){ 
    if(mHeatpadNextDuty != value) { 
        mHeatpadNextDuty = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void DebugScreenAbs::Model::setHeatpadNextPeriod(uint32_t value){ 
    if(mHeatpadNextPeriod != value) { 
        mHeatpadNextPeriod = value; 
        mScreen.markDirty(Model::Index::HeatpadDuty);
    } 
}

void DebugScreenAbs::Model::setHeatpadPwmProgress(float value){ 
    if(mHeatpadPwmProgress != value) { 
        mHeatpadPwmProgress = value; 
        mScreen.markDirty(Model::Index::BoxPosition);
    } 
}

void DebugScreenAbs::Model::setHeatpadMeasuredVoltage(float value){ 
    if(mHeatpadMeasuredVoltage != value) { 
        mHeatpadMeasuredVoltage = value; 
        mScreen.markDirty(Model::Index::HeatpadSense);
    } 
}

void DebugScreenAbs::Model::setHeatpadMeasuredCurrent(float value){ 
    if(mHeatpadMeasuredCurrent != value) { 
        mHeatpadMeasuredCurrent = value; 
        mScreen.markDirty(Model::Index::HeatpadSense);
    } 
}

void DebugScreenAbs::Model::setDisplayBrightness(float value){ 
    if(mDisplayBrightness != value) { 
        mDisplayBrightness = value; 
        mScreen.markDirty(Model::Index::DisplayStatus);
    } 
}

void DebugScreenAbs::Model::setDisplaySkipped(uint32_t value){ 
    if(mDisplaySkipped != value) { 
        mDisplaySkipped = value; 
        mScreen.markDirty(Model::Index::DisplayStatus);
    } 
}

void DebugScreenAbs::Model::setShtDriverEnabled(bool value){ 
    if(mShtDriverEnabled != value) { 
        mShtDriverEnabled = value; 
        mScreen.markDirty(Model::Index::TemperatureState);
    } 
}

void DebugScreenAbs::Model::setShtPowerEnabled(bool value){ 
    if(mShtPowerEnabled != value) { 
        mShtPowerEnabled = value; 
        mScreen.markDirty(Model::Index::TemperatureState);
    } 
}

void DebugScreenAbs::Model::setShtResetting(bool value){ 
    if(mShtResetting != value) { 
        mShtResetting = value; 
        mScreen.markDirty(Model::Index::TemperatureState);
    } 
}

void DebugScreenAbs::Model::setSensorTemperatureCelcius(float value){ 
    if(mSensorTemperatureCelcius != value) { 
        mSensorTemperatureCelcius = value; 
        mScreen.markDirty(Model::Index::TemperatureSample);
    } 
}

void DebugScreenAbs::Model::setSensorHumidityRelative(float value){ 
    if(mSensorHumidityRelative != value) { 
        mSensorHumidityRelative = value; 
        mScreen.markDirty(Model::Index::TemperatureSample);
    } 
}

void DebugScreenAbs::Model::setTimeSeconds(uint32_t value){ 
    if(mTimeSeconds != value) { 
        mTimeSeconds = value; 
        mScreen.markDirty(Model::Index::Time);
    } 
}

void DebugScreenAbs::Model::setEventCount(uint32_t value){ 
    if(mEventCount != value) { 
        mEventCount = value; 
        mScreen.markDirty(Model::Index::AppInfo);
    } 
}

void DebugScreenAbs::Model::setBehaviour(BehaviourId value){ 
    if(mBehaviour != value) { 
        mBehaviour = value; 
        mScreen.markDirty(Model::Index::AppInfo);
    } 
}

void DebugScreenAbs::Model::setEngineState(HeaterEngineState value){ 
    if(mEngineState != value) { 
        mEngineState = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void DebugScreenAbs::Model::setEngineTargetTemperature(float value){ 
    if(mEngineTargetTemperature != value) { 
        mEngineTargetTemperature = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void DebugScreenAbs::Model::setEngineMeasuredTemperature(float value){ 
    if(mEngineMeasuredTemperature != value) { 
        mEngineMeasuredTemperature = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void DebugScreenAbs::Model::setEngineMeasuredHumidity(float value){ 
    if(mEngineMeasuredHumidity != value) { 
        mEngineMeasuredHumidity = value; 
        mScreen.markDirty(Model::Index::FermentationStatus);
    } 
}

void DebugScreenAbs::Model::setHeapAllocatedBlocks(uint32_t value){ 
    if(mHeapAllocatedBlocks != value) { 
        mHeapAllocatedBlocks = value; 
        mScreen.markDirty(Model::Index::HeapBlocks);
    } 
}

void DebugScreenAbs::Model::setHeapFreeBlocks(uint32_t value){ 
    if(mHeapFreeBlocks != value) { 
        mHeapFreeBlocks = value; 
        mScreen.markDirty(Model::Index::HeapBlocks);
    } 
}

void DebugScreenAbs::Model::setHeapLargestFreeBlock(uint32_t value){ 
    if(mHeapLargestFreeBlock != value) { 
        mHeapLargestFreeBlock = value; 
        mScreen.markDirty(Model::Index::HeapBlocks);
    } 
}

void DebugScreenAbs::Model::setHeapAllocatedBytes(uint32_t value){ 
    if(mHeapAllocatedBytes != value) { 
        mHeapAllocatedBytes = value; 
        mScreen.markDirty(Model::Index::HeapBytes);
    } 
}

void DebugScreenAbs::Model::setHeapMinimumFreeBytes(uint32_t value){ 
    if(mHeapMinimumFreeBytes != value) { 
        mHeapMinimumFreeBytes = value; 
        mScreen.markDirty(Model::Index::HeapBytes);
    } 
}

void DebugScreenAbs::Model::setHeapTotalFreeBytes(uint32_t value){ 
    if(mHeapTotalFreeBytes != value) { 
        mHeapTotalFreeBytes = value; 
        mScreen.markDirty(Model::Index::HeapMinimum);
    } 
}

void DebugScreenAbs::Model::setHeapMinimumTime(uint32_t value){ 
    if(mHeapMinimumTime != value) { 
        mHeapMinimumTime = value; 
        mScreen.markDirty(Model::Index::HeapMinimum);
    } 
}

DebugScreenAbs::Model& DebugScreenAbs::model(){
    return mModel;
}

const DebugScreenAbs::Model& DebugScreenAbs::model() const {
    return mModel;
}

void DebugScreenAbs::markDirty(Model::Index index){
    mDirtyDispatcher.markDirty(static_cast<size_t>(index));
}

bool DebugScreenAbs::isMarkedDirty(Model::Index index) const {
    return mDirtyDispatcher.isMarkedDirty(static_cast<size_t>(index));
}

void DebugScreenAbs::applyFanStateTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyFanState();
}

void DebugScreenAbs::applyFanMeasuredRpmTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyFanMeasuredRpm();
}

void DebugScreenAbs::applyHeatpadStateTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyHeatpadState();
}

void DebugScreenAbs::applyHeatpadDutyTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyHeatpadDuty();
}

void DebugScreenAbs::applyBoxPositionTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyBoxPosition();
}

void DebugScreenAbs::applyHeatpadSenseTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyHeatpadSense();
}

void DebugScreenAbs::applyDisplayStatusTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyDisplayStatus();
}

void DebugScreenAbs::applyTemperatureStateTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyTemperatureState();
}

void DebugScreenAbs::applyTemperatureSampleTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyTemperatureSample();
}

void DebugScreenAbs::applyTimeTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyTime();
}

void DebugScreenAbs::applyAppInfoTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyAppInfo();
}

void DebugScreenAbs::applyFermentationStatusTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyFermentationStatus();
}

void DebugScreenAbs::applyHeapBlocksTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyHeapBlocks();
}

void DebugScreenAbs::applyHeapBytesTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyHeapBytes();
}

void DebugScreenAbs::applyHeapMinimumTrampoline(void* context){
    static_cast<DebugScreenAbs*>(context)->onApplyHeapMinimum();
}

} // namespace Garbox