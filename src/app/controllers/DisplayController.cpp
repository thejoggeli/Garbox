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
    mDisplay(PartsProvider::GetDisplay()){
    // nothing to do
}

void DisplayController::onInit(){
    mBacklightFader.setEasingFunction(EasingFunctions::GetOutSine());
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

        LvglObjects& lv = mDisplay.getLvglHandler().getObjects();

        if(mDirtyFlags.fanStatus){
            lv.setFanState(FanStateToString(mShadowState.fanState), mShadowState.fanTargetSpeed);
            mDirtyFlags.fanStatus = false;
        }
        if(mDirtyFlags.fanMeasuredRpm){
            lv.setFanMeasuredRpm(mShadowState.fanMeasuredRpm);
            mDirtyFlags.fanMeasuredRpm = false;
        }
        if(mDirtyFlags.heatpadState){
            lv.setHeatpadState(HeatpadStateToString(mShadowState.heatpadState));
            mDirtyFlags.heatpadState = false;
        }
        if(mDirtyFlags.heatpadDuty){ 
            lv.setHeatpadDuty(mShadowState.heatpadDuty);
            mDirtyFlags.heatpadDuty = false;
        }
        if(mDirtyFlags.heatpadSense){
            lv.setHeatpadSense(mShadowState.heatpadVoltage, mShadowState.heatpadCurrent);
            mDirtyFlags.heatpadSense = false;
        }
        if(mDirtyFlags.displayState){
            lv.setDisplayState(mShadowState.brightness, mShadowState.renderSkippedCount);
            mDirtyFlags.displayState = false;
        } 
        if(mDirtyFlags.shtState){
            lv.setTemperatureState(mShadowState.shtPower, mShadowState.shtDriver, mShadowState.shtReset);
            mDirtyFlags.shtState = false;
        }
        if(mDirtyFlags.shtSample){
            lv.setTemperatureSample(mShadowState.shtTemp, mShadowState.shtHum);
            mDirtyFlags.shtSample = false;
        }
        if(mDirtyFlags.heapSpace){
            lv.setHeapSpace(mShadowState.heapSpace);
            mDirtyFlags.heapSpace = false;
        }
        if(mDirtyFlags.appState){
            lv.setAppInfo(BehaviourIdToString(mShadowState.behaviour), mShadowState.eventCount);
            mDirtyFlags.appState = false;
        }

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