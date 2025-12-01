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
        writeHeapSpaceHeapSpace(heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        mHeapTimer.restart();
    }

    // update event count
    writeAppInfoEventCount(getContext()->eventCount);

    // last dispatched count must be updated manually, otherwise a dispatch is
    // triggered by itself on each tick
    if(mLastDispatchedCount != getDispatchedCount()){
        mLastDispatchedCount = getDispatchedCount();
        if(!isMarkedDirty(UpdaterIndex::DisplayStatus)){
            onApplyDisplayStatus();
        }
    }

    // update complete
    mFirstUpdate = false;
}

void MainScreen::onApplyFanState(){
    mObjects.setFanState(FanStateToString(mFanState.state), mFanState.targetSpeed);
}

void MainScreen::onApplyFanMeasuredRpm(){
    mObjects.setFanMeasuredRpm(mFanMeasuredRpm.measuredRpm);
}

void MainScreen::onApplyHeatpadState(){
    mObjects.setHeatpadState(HeatpadStateToString(mHeatpadState.state));
}

void MainScreen::onApplyHeatpadDuty(){
    mObjects.setHeatpadDuty(
        mHeatpadDuty.currentDutyCycle,
        mHeatpadDuty.nextDutyCycle,
        mHeatpadDuty.currentPeriodMicros,
        mHeatpadDuty.nextPeriodMicros
    );
}

void MainScreen::onApplyBoxPosition(){
    mObjects.setBoxPosition(static_cast<float>(mBoxPosition.pwmProgressMicros) / static_cast<float>(mHeatpadDuty.currentPeriodMicros));
}

void MainScreen::onApplyHeatpadSense(){
    mObjects.setHeatpadSense(mHeatpadSense.measuredVoltage, mHeatpadSense.measuredCurrent);
}

void MainScreen::onApplyDisplayStatus(){
    mObjects.setDisplayState(mDisplayStatus.brightness, mDisplayStatus.skipped, getDispatchedCount());
}

void MainScreen::onApplyTemperatureState(){
    mObjects.setTemperatureState(
        mTemperatureState.powerEnabled,
        mTemperatureState.driverEnabled,
        mTemperatureState.resetting
    );
}

void MainScreen::onApplyTemperatureSample(){
    mObjects.setTemperatureSample(
        mTemperatureSample.temperatureCelcius,
        mTemperatureSample.humidityRelative
    );
}

void MainScreen::onApplyHeapSpace(){
    mObjects.setHeapSpace(mHeapSpace.heapSpace);
}

void MainScreen::onApplyAppInfo(){
    mObjects.setAppInfo(BehaviourIdToString(mAppInfo.newBehaviour), mAppInfo.eventCount);
}

void MainScreen::onApplyFermentationStatus(){
    mObjects.setFermentationStatus(
        HeaterEngineStateToString(mFermentationStatus.heaterEngineState),
        mFermentationStatus.measuredTemperature,
        mFermentationStatus.targetTemperature
    );
}

} // namespace Garbox