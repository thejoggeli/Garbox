#include "DisplayController.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "modules/parts/display/Display.h"
#include "modules/parts/fan/Fan.h"
#include "modules/parts/heatpad/Heatpad.h"

namespace Garbox {

DisplayController::DisplayController(ControllerId id): 
    // init members
    ControllerAbs(id),
    mDisplay(PartsProvider::GetDisplay()){
    // nothing to do
}

void DisplayController::onInit(){
    // display already initialized in parts provider
}

void DisplayController::onStart(){
    // nothing to do
}

void DisplayController::onRenderTick(){
    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){

        LvglObjects& lv = mDisplay.getLvglHandler().getObjects();
        const Fan& fan = PartsProvider::GetFan();
        const Heatpad& heatpad = PartsProvider::GetHeatpad();

        State state;

        state.fanState = fan.getState();
        state.fanTargetSpeed = fan.getTargetSpeed();
        state.fanMeasuredRpm = fan.getMeasuredRpm();
        state.heatpadState = heatpad.getState();
        state.heatpadDuty = heatpad.getCurrentDutyCycle();
        state.heatpadVoltage = heatpad.getMeasuredVoltage();
        state.heatpadCurrent = heatpad.getMeasuredCurrent();
        state.renderSkippedCount = mRenderSkippedCount;

        if(state.fanState != mState.fanState) lv.setFanState(FanStateToString(state.fanState));
        if(state.fanTargetSpeed != mState.fanTargetSpeed) lv.setFanTargetSpeed(state.fanTargetSpeed);
        if(state.fanMeasuredRpm != mState.fanMeasuredRpm) lv.setFanMeasuredRpm(state.fanMeasuredRpm);
        if(state.heatpadState != mState.heatpadState) lv.setHeatpadState(HeatpadStateToString(state.heatpadState));
        if(state.heatpadDuty != mState.heatpadDuty) lv.setHeatpadDuty(state.heatpadDuty);
        if(state.heatpadVoltage != mState.heatpadVoltage) lv.setHeatpadVoltage(state.heatpadVoltage);
        if(state.heatpadCurrent != mState.heatpadCurrent) lv.setHeatpadCurrent(state.heatpadCurrent);
        if(state.renderSkippedCount != mState.renderSkippedCount) lv.setHeatpadCurrent(state.heatpadCurrent);

        mState = state;

        mDisplay.giveRenderTrigger();
    }
    else {
        mRenderSkippedCount++;
        LogDebug("DisplayController", "render skipped! total skip count = %" PRIi32, mRenderSkippedCount);
    }
}

} // namespace