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
    // nothing to do
}

void MainScreen::onStart(){
    mHeapTimer.start(1000_ms);
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
        mModel.setHeapSpace(heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        mHeapTimer.restart();
    }

    // update event count
    mModel.setEventCount(getContext()->eventCount);

    // last dispatched count must be updated manually, otherwise a dispatch is
    // triggered by itself on each tick
    if(mLastDispatchedCount != getDispatchedCount()){
        mLastDispatchedCount = getDispatchedCount();
        if(!isMarkedDirty(Model::Index::DisplayStatus)){
            onApplyDisplayStatus();
        }
    }

    // update complete
    mFirstUpdate = false;
}

void MainScreen::onApplyFanState(){
    mObjects.setFanState(FanStateToString(mModel.getFanState()), mModel.getFanTargetSpeed());
}

void MainScreen::onApplyFanMeasuredRpm(){
    mObjects.setFanMeasuredRpm(mModel.getFanMeasuredRpm());
}

void MainScreen::onApplyHeatpadState(){
    mObjects.setHeatpadState(HeatpadStateToString(mModel.getHeatpadState()));
}

void MainScreen::onApplyHeatpadDuty(){
    mObjects.setHeatpadDuty(
        mModel.getHeatpadCurrentDuty(),
        mModel.getHeatpadNextDuty(),
        mModel.getHeatpadCurrentPeriod(),
        mModel.getHeatpadNextPeriod()
    );
}

void MainScreen::onApplyBoxPosition(){
    mObjects.setBoxPosition(static_cast<float>(mModel.getHeatpadPwmProgress()) / static_cast<float>(mModel.getHeatpadCurrentPeriod()));
}

void MainScreen::onApplyHeatpadSense(){
    mObjects.setHeatpadSense(mModel.getHeatpadMeasuredVoltage(), mModel.getHeatpadMeasuredCurrent());
}

void MainScreen::onApplyDisplayStatus(){
    mObjects.setDisplayState(mModel.getDisplayBrightness(), mModel.getDisplaySkipped(), getDispatchedCount());
}

void MainScreen::onApplyTemperatureState(){
    mObjects.setTemperatureState(
        mModel.getShtPowerEnabled(),
        mModel.getShtDriverEnabled(),
        mModel.getShtResetting()
    );
}

void MainScreen::onApplyTemperatureSample(){
    mObjects.setTemperatureSample(
        mModel.getSensorTemperatureCelcius(),
        mModel.getSensorHumidityRelative()
    );
}

void MainScreen::onApplyHeapSpace(){
    mObjects.setHeapSpace(mModel.getHeapSpace());
}

void MainScreen::onApplyAppInfo(){
    mObjects.setAppInfo(BehaviourIdToString(mModel.getBehaviour()), mModel.getEventCount());
}

void MainScreen::onApplyFermentationStatus(){
    mObjects.setFermentationStatus(
        HeaterEngineStateToString(mModel.getEngineState()),
        mModel.getEngineMeasuredTemperature(),
        mModel.getEngineTargetTemperature()
    );
}

} // namespace Garbox