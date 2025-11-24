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
    registerHandler([this](){ mObjects.setFanState(FanStateToString(mShadowState.fanState), mShadowState.fanTargetSpeed); });
    registerHandler([this](){ mObjects.setFanMeasuredRpm(mShadowState.fanMeasuredRpm); });
    registerHandler([this](){ mObjects.setHeatpadState(HeatpadStateToString(mShadowState.heatpadState)); });
    registerHandler([this](){ mObjects.setHeatpadDuty(mShadowState.heatpadDuty, mShadowState.heatpadPeriod); });
    registerHandler([this](){ mObjects.setHeatpadSense(mShadowState.heatpadVoltage, mShadowState.heatpadCurrent); });
    registerHandler([this](){ mObjects.setDisplayState(mShadowState.brightness, mRenderSkippedCount, mDirtyCount); });
    registerHandler([this](){ mObjects.setTemperatureState(mShadowState.shtPower, mShadowState.shtDriver, mShadowState.shtReset); });
    registerHandler([this](){ mObjects.setTemperatureSample(mShadowState.shtTemp, mShadowState.shtHum); });
    registerHandler([this](){ mObjects.setHeapSpace(mShadowState.heapSpace); });
    registerHandler([this](){ mObjects.setAppInfo(BehaviourIdToString(mShadowState.behaviour), mShadowState.eventCount); });
}

void DisplayController::registerHandler(UpdateFunction function){
    const bool dirty = true;
    UpdateHandler* handler = mUpdateHandlers.emplace(dirty, function);
    mDirtyUpdateHandlers.push(handler);
}

void DisplayController::markDirty(Index index){
    UpdateHandler& handler = mUpdateHandlers[static_cast<size_t>(index)];
    if(!handler.dirty){
        handler.dirty = true;
        mDirtyCount++;
        bool ok = mDirtyUpdateHandlers.push(&handler) ;
        AssertDebug(ok, "DisplayController", "push update handler failed");
        markDirty(Index::DisplayState);
    } 
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
        markDirty(Index::DisplayState);
    }

    // update heap space
    if(mHeapTimer.isExpired()){
        uint32_t space = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
        if(space != mShadowState.heapSpace){
            markDirty(Index::HeapSpace);
            mShadowState.heapSpace = space;
        }
        mHeapTimer.restart();
    }

    // update event count
    if(mShadowState.eventCount != mContext->eventCount){
        mShadowState.eventCount = mContext->eventCount;
        markDirty(Index::AppState);
    }

    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){

        if(mDirtyUpdateHandlers.size() > 0){
            // update all dirty handlers
            for(UpdateHandler* handler : mDirtyUpdateHandlers){
                handler->dirty = false;
                handler->updateFn();
            }
            mDirtyUpdateHandlers.releaseAll();
        }

        // signal display to render frame
        mDisplay.giveRenderTrigger();
    }
    else {
        mRenderSkippedCount++;
        markDirty(Index::DisplayState);
        LogDebug("DisplayController", "render skipped! total skip count = %" PRIi32, mRenderSkippedCount);
    }
}

void DisplayController::onFanStatus(const FanStatusEvent& event){ 
    mShadowState.fanState = event->state;
    mShadowState.fanTargetSpeed = event->targetSpeed;
    markDirty(Index::FanStatus);
}

void DisplayController::onFanSample(const FanSampleEvent& event){ 
    mShadowState.fanMeasuredRpm = event->measuredRpm;
    markDirty(Index::FanMeasuredRpm);
}

void DisplayController::onHeatpadStatus(const HeatpadStatusEvent& event){
    mShadowState.heatpadDuty = event->dutyCycle;
    mShadowState.heatpadState = event->state;
    mShadowState.heatpadPeriod = event->periodMicros;
    markDirty(Index::HeatpadState);
    markDirty(Index::HeatpadDuty);
}

void DisplayController::onHeatpadSample(const HeatpadSampleEvent& event){
    mShadowState.heatpadVoltage = event->measuredVoltage;
    mShadowState.heatpadCurrent = event->measuredCurrent;
    markDirty(Index::HeatpadSense);
}

void DisplayController::onTemperatureStatus(const TemperatureStatusEvent& event){
    mShadowState.shtDriver = event->driverEnabled;
    mShadowState.shtPower = event->powerEnabled;
    mShadowState.shtReset = event->resetting;
    markDirty(Index::ShtState);
}

void DisplayController::onTemperatureSample(const TemperatureSampleEvent& event){
    mShadowState.shtTemp = event->temperatureCelcius;
    mShadowState.shtHum = event->humidityRelative;
    markDirty(Index::ShtSample);
}
void DisplayController::onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event){
    mShadowState.behaviour = event->newBehaviour;
    markDirty(Index::AppState);
}

void DisplayController::onBacklightCommand(const BacklightCommandEvent& event){
    setBrightnessSmooth(event->brightness, 1000_ms);
};

void DisplayController::setBrightnessSmooth(float targetBrightness, uint32_t durationMicros){
    const float startBrightness = mDisplay.getBrightness();
    mBacklightFader.start(startBrightness, targetBrightness, durationMicros);
}

} // namespace