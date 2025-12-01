// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs() : ScreenAbs(ComponentId::MainScreen, ScreenId::Main){
    // nothing to do
}

void MainScreenAbs::initScreen(){
    ScreenAbs::initScreen();
    mDirtyDispatcher.registerHandler(applyFanStateTrampoline, this);
    mDirtyDispatcher.registerHandler(applyFanMeasuredRpmTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadStateTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadDutyTrampoline, this);
    mDirtyDispatcher.registerHandler(applyBoxPositionTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeatpadSenseTrampoline, this);
    mDirtyDispatcher.registerHandler(applyDisplayStatusTrampoline, this);
    mDirtyDispatcher.registerHandler(applyTemperatureStateTrampoline, this);
    mDirtyDispatcher.registerHandler(applyTemperatureSampleTrampoline, this);
    mDirtyDispatcher.registerHandler(applyHeapSpaceTrampoline, this);
    mDirtyDispatcher.registerHandler(applyAppInfoTrampoline, this);
    mDirtyDispatcher.registerHandler(applyFermentationStatusTrampoline, this);
}

void MainScreenAbs::updateScreen(){
    ScreenAbs::updateScreen();
    mDispatchedCount += mDirtyDispatcher.getDirtyCount();
    mDirtyDispatcher.dispatch();
}

DisplayCommandEvent MainScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void MainScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

void MainScreenAbs::markDirty(UpdaterIndex index){
    mDirtyDispatcher.markDirty(static_cast<size_t>(index));
}

void MainScreenAbs::receiveFanStatus(const FanStatusEvent& event){
    bool fanStateChanged = false;
    if(mFanState.state != event->state){
        mFanState.state = event->state;
        fanStateChanged = true;
    }
    if(mFanState.targetSpeed != event->targetSpeed){
        mFanState.targetSpeed = event->targetSpeed;
        fanStateChanged = true;
    }
    if(fanStateChanged){
        markDirty(UpdaterIndex::FanState);
    }
}

void MainScreenAbs::receiveFanSample(const FanSampleEvent& event){
    bool fanMeasuredRpmChanged = false;
    if(mFanMeasuredRpm.measuredRpm != event->measuredRpm){
        mFanMeasuredRpm.measuredRpm = event->measuredRpm;
        fanMeasuredRpmChanged = true;
    }
    if(fanMeasuredRpmChanged){
        markDirty(UpdaterIndex::FanMeasuredRpm);
    }
}

void MainScreenAbs::receiveHeatpadStatus(const HeatpadStatusEvent& event){
    bool heatpadDutyChanged = false;
    bool heatpadStateChanged = false;
    if(mHeatpadState.state != event->state){
        mHeatpadState.state = event->state;
        heatpadStateChanged = true;
    }
    if(mHeatpadDuty.currentDutyCycle != event->currentDutyCycle){
        mHeatpadDuty.currentDutyCycle = event->currentDutyCycle;
        heatpadDutyChanged = true;
    }
    if(mHeatpadDuty.currentPeriodMicros != event->currentPeriodMicros){
        mHeatpadDuty.currentPeriodMicros = event->currentPeriodMicros;
        heatpadDutyChanged = true;
    }
    if(mHeatpadDuty.nextDutyCycle != event->nextDutyCycle){
        mHeatpadDuty.nextDutyCycle = event->nextDutyCycle;
        heatpadDutyChanged = true;
    }
    if(mHeatpadDuty.nextPeriodMicros != event->nextPeriodMicros){
        mHeatpadDuty.nextPeriodMicros = event->nextPeriodMicros;
        heatpadDutyChanged = true;
    }
    if(heatpadDutyChanged){
        markDirty(UpdaterIndex::HeatpadDuty);
    }
    if(heatpadStateChanged){
        markDirty(UpdaterIndex::HeatpadState);
    }
}

void MainScreenAbs::receiveHeatpadSample(const HeatpadSampleEvent& event){
    bool boxPositionChanged = false;
    bool heatpadSenseChanged = false;
    if(mBoxPosition.pwmProgressMicros != event->pwmProgressMicros){
        mBoxPosition.pwmProgressMicros = event->pwmProgressMicros;
        boxPositionChanged = true;
    }
    if(mHeatpadSense.measuredVoltage != event->measuredVoltage){
        mHeatpadSense.measuredVoltage = event->measuredVoltage;
        heatpadSenseChanged = true;
    }
    if(mHeatpadSense.measuredCurrent != event->measuredCurrent){
        mHeatpadSense.measuredCurrent = event->measuredCurrent;
        heatpadSenseChanged = true;
    }
    if(boxPositionChanged){
        markDirty(UpdaterIndex::BoxPosition);
    }
    if(heatpadSenseChanged){
        markDirty(UpdaterIndex::HeatpadSense);
    }
}

void MainScreenAbs::receiveDisplayStatus(const DisplayStatusEvent& event){
    bool displayStatusChanged = false;
    if(mDisplayStatus.brightness != event->brightness){
        mDisplayStatus.brightness = event->brightness;
        displayStatusChanged = true;
    }
    if(mDisplayStatus.skipped != event->skipped){
        mDisplayStatus.skipped = event->skipped;
        displayStatusChanged = true;
    }
    if(displayStatusChanged){
        markDirty(UpdaterIndex::DisplayStatus);
    }
}

void MainScreenAbs::receiveTemperatureStatus(const TemperatureStatusEvent& event){
    bool temperatureStateChanged = false;
    if(mTemperatureState.driverEnabled != event->driverEnabled){
        mTemperatureState.driverEnabled = event->driverEnabled;
        temperatureStateChanged = true;
    }
    if(mTemperatureState.powerEnabled != event->powerEnabled){
        mTemperatureState.powerEnabled = event->powerEnabled;
        temperatureStateChanged = true;
    }
    if(mTemperatureState.resetting != event->resetting){
        mTemperatureState.resetting = event->resetting;
        temperatureStateChanged = true;
    }
    if(temperatureStateChanged){
        markDirty(UpdaterIndex::TemperatureState);
    }
}

void MainScreenAbs::receiveTemperatureSample(const TemperatureSampleEvent& event){
    bool temperatureSampleChanged = false;
    if(mTemperatureSample.temperatureCelcius != event->temperatureCelcius){
        mTemperatureSample.temperatureCelcius = event->temperatureCelcius;
        temperatureSampleChanged = true;
    }
    if(mTemperatureSample.humidityRelative != event->humidityRelative){
        mTemperatureSample.humidityRelative = event->humidityRelative;
        temperatureSampleChanged = true;
    }
    if(temperatureSampleChanged){
        markDirty(UpdaterIndex::TemperatureSample);
    }
}

void MainScreenAbs::receiveActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event){
    bool appInfoChanged = false;
    if(mAppInfo.newBehaviour != event->newBehaviour){
        mAppInfo.newBehaviour = event->newBehaviour;
        appInfoChanged = true;
    }
    if(appInfoChanged){
        markDirty(UpdaterIndex::AppInfo);
    }
}

void MainScreenAbs::receiveFermentationStatus(const FermentationStatusEvent& event){
    bool fermentationStatusChanged = false;
    if(mFermentationStatus.heaterEngineState != event->heaterEngineState){
        mFermentationStatus.heaterEngineState = event->heaterEngineState;
        fermentationStatusChanged = true;
    }
    if(mFermentationStatus.targetTemperature != event->targetTemperature){
        mFermentationStatus.targetTemperature = event->targetTemperature;
        fermentationStatusChanged = true;
    }
    if(mFermentationStatus.measuredTemperature != event->measuredTemperature){
        mFermentationStatus.measuredTemperature = event->measuredTemperature;
        fermentationStatusChanged = true;
    }
    if(mFermentationStatus.measuredHumidity != event->measuredHumidity){
        mFermentationStatus.measuredHumidity = event->measuredHumidity;
        fermentationStatusChanged = true;
    }
    if(fermentationStatusChanged){
        markDirty(UpdaterIndex::FermentationStatus);
    }
}

void MainScreenAbs::writeHeapSpaceHeapSpace(uint32_t heapSpace){
    if(mHeapSpace.heapSpace != heapSpace){
        mHeapSpace.heapSpace = heapSpace;
        markDirty(UpdaterIndex::HeapSpace);
    }
}
void MainScreenAbs::writeAppInfoEventCount(uint32_t eventCount){
    if(mAppInfo.eventCount != eventCount){
        mAppInfo.eventCount = eventCount;
        markDirty(UpdaterIndex::AppInfo);
    }
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

void MainScreenAbs::applyHeapSpaceTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyHeapSpace();
}

void MainScreenAbs::applyAppInfoTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyAppInfo();
}

void MainScreenAbs::applyFermentationStatusTrampoline(void* context){
    static_cast<MainScreenAbs*>(context)->onApplyFermentationStatus();
}

} // namespace Garbox