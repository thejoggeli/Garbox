// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "MainScreen.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "modules/parts/display/Display.h"

namespace Garbox {

MainScreen::MainScreen(): 
    MainScreenAbs(),
    mObjects(PartsProvider::GetDisplay().getLvglHandler().getObjects()){
    // nothing to do
}

void MainScreen::onInit(){
    mDirtyDispatcher.registerHandler(onUpdateFanState, this);
    mDirtyDispatcher.registerHandler(onUpdateFanMeasuredRpm, this);
    mDirtyDispatcher.registerHandler(onUpdateHeatpadState, this);
    mDirtyDispatcher.registerHandler(onUpdateHeatpadDuty, this);
    mDirtyDispatcher.registerHandler(onUpdateBoxPosition, this);
    mDirtyDispatcher.registerHandler(onUpdateHeatpadSense, this);
    mDirtyDispatcher.registerHandler(onUpdateDisplayStatus, this);
    mDirtyDispatcher.registerHandler(onUpdateTemperatureState, this);
    mDirtyDispatcher.registerHandler(onUpdateTemperatureSample, this);
    mDirtyDispatcher.registerHandler(onUpdateHeapSpace, this);
    mDirtyDispatcher.registerHandler(onUpdateAppInfo, this);
    mDirtyDispatcher.registerHandler(onUpdateFermentationStatus, this);
}

void MainScreen::onStart(){
    mHeapTimer.start(1000_ms);
    mDirtyDispatcher.markAllDirty();
}

void MainScreen::onBecomeEnabled(){
    mFirstUpdate = true;
}

void MainScreen::onBecomeDisabled(){
    // nothing to do
}

void MainScreen::onUpdateScreen(){

    if(mFirstUpdate){
        mObjects.setBackgroundColor(0x0);
    }

    // update heap space
    if(mHeapTimer.isExpired()){
        uint32_t space = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
        if(space != mShadowState.heapSpace){
            mDirtyDispatcher.markDirty(static_cast<size_t>(Index::HeapSpace));
            mShadowState.heapSpace = space;
        }
        mHeapTimer.restart();
    }

    // update event count
    if(mShadowState.eventCount != getContext()->eventCount){
        mShadowState.eventCount = getContext()->eventCount;
        mDirtyDispatcher.markDirty(static_cast<size_t>(Index::AppState));
    }

    // update dirty count
    uint32_t count = mShadowState.dirtyCount + mDirtyDispatcher.getDirtyCount();
    if(mShadowState.dirtyCount != count){
        mShadowState.dirtyCount = count;
        mDirtyDispatcher.markDirty(static_cast<size_t>(Index::DisplayStatus));
    }

    // dispatch dirty
    mDirtyDispatcher.dispatch();
    mFirstUpdate = false;
}

void MainScreen::onFanStatus(const FanStatusEvent& event){ 
    mShadowState.fanState = event->state;
    mShadowState.fanTargetSpeed = event->targetSpeed;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::FanStatus));
}

void MainScreen::onFanSample(const FanSampleEvent& event){ 
    mShadowState.fanMeasuredRpm = event->measuredRpm;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::FanMeasuredRpm));
}

void MainScreen::onHeatpadStatus(const HeatpadStatusEvent& event){
    if (mShadowState.heatpadCurrentDuty != event->currentDutyCycle ||
        mShadowState.heatpadCurrentPeriod != event->currentPeriodMicros ||
        mShadowState.heatpadNextDuty != event->nextDutyCycle ||
        mShadowState.heatpadNextPeriod != event->nextPeriodMicros){
        // update shadow state
        mShadowState.heatpadCurrentDuty = event->currentDutyCycle;
        mShadowState.heatpadCurrentPeriod = event->currentPeriodMicros;
        mShadowState.heatpadNextDuty = event->nextDutyCycle;
        mShadowState.heatpadNextPeriod = event->nextPeriodMicros;
        mDirtyDispatcher.markDirty(static_cast<size_t>(Index::HeatpadDuty));
    }
    if(mShadowState.heatpadState != event->state){
        mShadowState.heatpadState = event->state;
        mDirtyDispatcher.markDirty(static_cast<size_t>(Index::HeatpadState));
    }
}

void MainScreen::onHeatpadSample(const HeatpadSampleEvent& event){
    mShadowState.heatpadPwmProgressMicros = event->pwmProgressMicros;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::HeatpadProgress));
    mShadowState.heatpadMeasuredVoltage = event->measuredVoltage;
    mShadowState.heatpadMeasuredCurrent = event->measuredCurrent;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::HeatpadSense));
}

void MainScreen::onTemperatureStatus(const TemperatureStatusEvent& event){
    mShadowState.shtDriver = event->driverEnabled;
    mShadowState.shtPower = event->powerEnabled;
    mShadowState.shtReset = event->resetting;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::ShtState));
}

void MainScreen::onTemperatureSample(const TemperatureSampleEvent& event){
    mShadowState.shtTemp = event->temperatureCelcius;
    mShadowState.shtHum = event->humidityRelative;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::ShtSample));
}

void MainScreen::onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event){
    mShadowState.behaviour = event->newBehaviour;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::AppState));
}

void MainScreen::onFermentationStatus(const FermentationStatusEvent& event){
    if (mShadowState.engineState == event->heaterEngineState &&
        mShadowState.engineTargetTemp == event->targetTemperature &&
        mShadowState.engineMeasuredTemp == event->measuredTemperature &&
        mShadowState.engineMeasuredHum == event->measuredHumidity){
        return; 
    }
    mShadowState.engineState = event->heaterEngineState;
    mShadowState.engineTargetTemp = event->targetTemperature;
    mShadowState.engineMeasuredTemp = event->measuredTemperature;
    mShadowState.engineMeasuredHum = event->measuredHumidity;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::FermentationStatus));
}

void MainScreen::onDisplayStatus(const DisplayStatusEvent& event){
    if (mShadowState.brightness == event->brightness &&
        mShadowState.renderSkippedCount == event->skipped){
        return; 
    }
    mShadowState.brightness = event->brightness;
    mShadowState.renderSkippedCount = event->skipped;
    mDirtyDispatcher.markDirty(static_cast<size_t>(Index::DisplayStatus));
}

void MainScreen::onUpdateFanState(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setFanState(
        FanStateToString(self->mShadowState.fanState),
        self->mShadowState.fanTargetSpeed
    );
}

void MainScreen::onUpdateFanMeasuredRpm(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setFanMeasuredRpm(self->mShadowState.fanMeasuredRpm);
}

void MainScreen::onUpdateHeatpadState(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setHeatpadState(
        HeatpadStateToString(self->mShadowState.heatpadState)
    );
}

void MainScreen::onUpdateHeatpadDuty(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setHeatpadDuty(
        self->mShadowState.heatpadCurrentDuty,
        self->mShadowState.heatpadNextDuty,
        self->mShadowState.heatpadCurrentPeriod,
        self->mShadowState.heatpadNextPeriod
    );
}

void MainScreen::onUpdateBoxPosition(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setBoxPosition(
        static_cast<float>(self->mShadowState.heatpadPwmProgressMicros) /
        static_cast<float>(self->mShadowState.heatpadCurrentPeriod)
    );
}

void MainScreen::onUpdateHeatpadSense(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setHeatpadSense(
        self->mShadowState.heatpadMeasuredVoltage,
        self->mShadowState.heatpadMeasuredCurrent
    );
}

void MainScreen::onUpdateDisplayStatus(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setDisplayState(
        self->mShadowState.brightness,
        self->mShadowState.renderSkippedCount,
        self->mShadowState.dirtyCount
    );
}

void MainScreen::onUpdateTemperatureState(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setTemperatureState(
        self->mShadowState.shtPower,
        self->mShadowState.shtDriver,
        self->mShadowState.shtReset
    );
}

void MainScreen::onUpdateTemperatureSample(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setTemperatureSample(
        self->mShadowState.shtTemp,
        self->mShadowState.shtHum
    );
}

void MainScreen::onUpdateHeapSpace(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setHeapSpace(self->mShadowState.heapSpace);
}

void MainScreen::onUpdateAppInfo(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setAppInfo(
        BehaviourIdToString(self->mShadowState.behaviour),
        self->mShadowState.eventCount
    );
}

void MainScreen::onUpdateFermentationStatus(void* context){
    auto* self = static_cast<MainScreen*>(context);
    self->mObjects.setFermentationStatus(
        HeaterEngineStateToString(self->mShadowState.engineState),
        self->mShadowState.engineMeasuredTemp,
        self->mShadowState.engineTargetTemp
    );
}

} // namespace Garbox