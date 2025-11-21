#include "DisplayController.h"

#include <esp_heap_caps.h>
#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "core/util/function/default/EasingFunctions.h"
#include "core/util/math/MathUtils.h"
#include "modules/parts/display/Display.h"
#include "modules/parts/fan/Fan.h"
#include "modules/parts/heatpad/Heatpad.h"

namespace Garbox {

DisplayController::DisplayController(const RuntimeContext& context): 
    // init members
    DisplayControllerAbs(context),
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

    if(mBacklightFader.isActive()){
        float brightness = mBacklightFader.updateValue();
        mDisplay.setBrightness(brightness);
    }

    if(mHeapTimer.isExpired()){
        mNewState.heapSpace = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
        mHeapTimer.restart();
    }

    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){

        LvglObjects& lv = mDisplay.getLvglHandler().getObjects();
        const Fan& fan = PartsProvider::GetFan();
        const Heatpad& heatpad = PartsProvider::GetHeatpad();

        mNewState.fanState = fan.getState();
        mNewState.fanTargetSpeed = fan.getTargetSpeed();
        mNewState.fanMeasuredRpm = fan.getMeasuredRpm();
        mNewState.heatpadState = heatpad.getState();
        mNewState.heatpadDuty = heatpad.getCurrentDutyCycle();
        mNewState.heatpadVoltage = heatpad.getMeasuredVoltage();
        mNewState.heatpadCurrent = heatpad.getMeasuredCurrent();
        mNewState.renderSkippedCount = mRenderSkippedCount;
        mNewState.brightness = mDisplay.getBrightness();

        if(mNewState.fanState != mOldState.fanState || mNewState.fanTargetSpeed != mOldState.fanTargetSpeed){
            lv.setFanState(FanStateToString(mNewState.fanState), mNewState.fanTargetSpeed);
        }
        if(mNewState.fanMeasuredRpm != mOldState.fanMeasuredRpm){
            lv.setFanMeasuredRpm(mNewState.fanMeasuredRpm);
        }
        if(mNewState.heatpadState != mOldState.heatpadState){
            lv.setHeatpadState(HeatpadStateToString(mNewState.heatpadState));
        }
        if(mNewState.heatpadDuty != mOldState.heatpadDuty){ 
            lv.setHeatpadDuty(mNewState.heatpadDuty);
        }
        if(mNewState.heatpadVoltage != mOldState.heatpadVoltage || mNewState.heatpadCurrent != mOldState.heatpadCurrent){
            lv.setHeatpadSense(mNewState.heatpadVoltage, mNewState.heatpadCurrent);
        }
        if(mNewState.renderSkippedCount != mOldState.renderSkippedCount || mNewState.brightness != mOldState.brightness){
            lv.setDisplayState(mNewState.brightness, mNewState.renderSkippedCount);
        } 
        if(mDirty.shtState){
            lv.setTemperatureState(mNewState.shtPower, mNewState.shtDriver, mNewState.shtReset);
            mDirty.shtState = false;
        }
        if(mDirty.shtSample){
            lv.setTemperatureSample(mNewState.shtTemp, mNewState.shtHum);
            mDirty.shtSample = false;
        }
        if(mNewState.heapSpace != mOldState.heapSpace){
            lv.setHeapSpace(mNewState.heapSpace);
        }

        mOldState = mNewState;

        mDisplay.giveRenderTrigger();
    }
    else {
        mRenderSkippedCount++;
        LogDebug("DisplayController", "render skipped! total skip count = %" PRIi32, mRenderSkippedCount);
    }
}

void DisplayController::onTemperatureStatus(const TemperatureStatus& event){
    mNewState.shtDriver = event.payload->driverEnabled;
    mNewState.shtPower = event.payload->powerEnabled;
    mNewState.shtReset = event.payload->resetting;
    mDirty.shtState = true;
}

void DisplayController::onTemperatureSample(const TemperatureSample& event){
    mNewState.shtTemp = event.payload->temperatureCelcius;
    mNewState.shtHum = event.payload->humidityRelative;
    mDirty.shtSample = true;
}

void DisplayController::onBacklightCommand(const BacklightCommand& event){
    setBrightnessSmooth(event.payload->brightness, 1000_ms);
};

void DisplayController::setBrightnessSmooth(float targetBrightness, uint32_t durationMicros){
    const float startBrightness = mDisplay.getBrightness();
    mBacklightFader.start(startBrightness, targetBrightness, durationMicros);
}

} // namespace