#include "DisplayController.h"

#include <esp_heap_caps.h>
#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "core/util/function/default/EasingFunctions.h"
#include "core/util/math/MathUtils.h"
#include "modules/parts/display/Display.h"

namespace Garbox {

DisplayController::DisplayController(): 
    // init members
    DisplayControllerAbs(),
    mDisplay(PartsProvider::GetDisplay()),
    mObjects(mDisplay.getLvglHandler().getObjects()){
    // nothing to do
}

void DisplayController::onInit(){
    mBacklightFader.setEasingFunction(EasingFunctions::GetOutSine());
    registerHandlers();
}

void DisplayController::registerHandlers(){
    mUpdateHandlers.emplace(mDirtyFlags.fanStatus, [this](){
        mObjects.setFanState(FanStateToString(mShadowState.fanState), mShadowState.fanTargetSpeed);
    });
    mUpdateHandlers.emplace(mDirtyFlags.fanMeasuredRpm, [this](){
        mObjects.setFanMeasuredRpm(mShadowState.fanMeasuredRpm);
    });
    mUpdateHandlers.emplace(mDirtyFlags.heatpadState, [this](){
        mObjects.setHeatpadState(HeatpadStateToString(mShadowState.heatpadState));
    });
    mUpdateHandlers.emplace(mDirtyFlags.heatpadDuty, [this](){
        mObjects.setHeatpadDuty(mShadowState.heatpadDuty);
    });
    mUpdateHandlers.emplace(mDirtyFlags.heatpadSense, [this](){
        mObjects.setHeatpadSense(mShadowState.heatpadVoltage, mShadowState.heatpadCurrent);
    });
    mUpdateHandlers.emplace(mDirtyFlags.displayState, [this](){
        mObjects.setDisplayState(mShadowState.brightness, mShadowState.renderSkippedCount);
    });
    mUpdateHandlers.emplace(mDirtyFlags.shtState, [this](){
        mObjects.setTemperatureState(mShadowState.shtPower, mShadowState.shtDriver, mShadowState.shtReset);
    });
    mUpdateHandlers.emplace(mDirtyFlags.shtSample, [this](){
        mObjects.setTemperatureSample(mShadowState.shtTemp, mShadowState.shtHum);
    });
    mUpdateHandlers.emplace(mDirtyFlags.heapSpace, [this](){
        mObjects.setHeapSpace(mShadowState.heapSpace);
    });
    mUpdateHandlers.emplace(mDirtyFlags.appState, [this](){
        mObjects.setAppInfo(BehaviourIdToString(mShadowState.behaviour), mShadowState.eventCount);
    });
}

void DisplayController::onStart(){
    mHeapTimer.start(1000_ms);
    setBrightnessSmooth(0.65f, 1500_ms);
}

void DisplayController::onRenderTick(){

    // update display brightness
    if(mBacklightFader.isActive()){
        float brightness = mBacklightFader.updateValue();
        mDisplay.setBrightness(brightness);
        mShadowState.brightness = brightness;
        mDirtyFlags.displayState = true;
    }

    // update heap space
    if(mHeapTimer.isExpired()){
        uint32_t space = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
        if(space != mShadowState.heapSpace){
            mDirtyFlags.heapSpace = true;
            mShadowState.heapSpace = space;
        }
        mHeapTimer.restart();
    }

    // update event count
    if(mShadowState.eventCount != mContext->eventCount){
        mShadowState.eventCount = mContext->eventCount;
        mDirtyFlags.appState = true;
    }

    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){

        // update all dirty handlers
        for(UpdateHandler& handler : mUpdateHandlers){
            if(handler.dirtyFlag){
                handler.updateFn();
                handler.dirtyFlag = false;
            }
        }

        // signal display to render frame
        mDisplay.giveRenderTrigger();
    }
    else {
        mRenderSkippedCount++;
        mDirtyFlags.displayState = true;
        LogDebug("DisplayController", "render skipped! total skip count = %" PRIi32, mRenderSkippedCount);
    }
}

void DisplayController::onFanStatus(const FanStatusEvent& event){ 
    if(mShadowState.fanState != event->state || mShadowState.fanTargetSpeed != event->targetSpeed){
        mDirtyFlags.fanStatus = true;
        mShadowState.fanState = event->state;
        mShadowState.fanTargetSpeed = event->targetSpeed;
    }
}

void DisplayController::onFanSample(const FanSampleEvent& event){ 
    if(mShadowState.fanMeasuredRpm != event->measuredRpm){
        mDirtyFlags.fanMeasuredRpm = true;
        mShadowState.fanMeasuredRpm = true;
    }
}

void DisplayController::onHeatpadStatus(const HeatpadStatusEvent& event){
    mShadowState.heatpadDuty = event->dutyCycle;
    mShadowState.heatpadState = event->state;
    mShadowState.heatpadPeriod = event->periodMicros;
    mDirtyFlags.heatpadState = true;
    mDirtyFlags.heatpadDuty = true;
}

void DisplayController::onHeatpadSample(const HeatpadSampleEvent& event){
    mShadowState.heatpadVoltage = event->measuredVoltage;
    mShadowState.heatpadCurrent = event->measuredCurrent;
    mDirtyFlags.heatpadSense = true;
}

void DisplayController::onTemperatureStatus(const TemperatureStatusEvent& event){
    mShadowState.shtDriver = event->driverEnabled;
    mShadowState.shtPower = event->powerEnabled;
    mShadowState.shtReset = event->resetting;
    mDirtyFlags.shtState = true;
}

void DisplayController::onTemperatureSample(const TemperatureSampleEvent& event){
    mShadowState.shtTemp = event->temperatureCelcius;
    mShadowState.shtHum = event->humidityRelative;
    mDirtyFlags.shtSample = true;
}

void DisplayController::onBacklightCommand(const BacklightCommandEvent& event){
    setBrightnessSmooth(event->brightness, 1000_ms);
};

void DisplayController::onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event){
    mShadowState.behaviour = event->newBehaviour;
    mDirtyFlags.appState = true;
}

void DisplayController::setBrightnessSmooth(float targetBrightness, uint32_t durationMicros){
    const float startBrightness = mDisplay.getBrightness();
    mBacklightFader.start(startBrightness, targetBrightness, durationMicros);
}

} // namespace