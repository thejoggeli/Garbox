#include "DisplayController.h"

#include "app/providers/PartsProvider.h"
#include "parts/display/Display.h"
#include "parts/fan/Fan.h"
#include "parts/heatpad/Heatpad.h"

namespace Garbox {

DisplayController::DisplayController(): 
    // init members
    ControllerAbs(ControllerId::Display),
    mDisplay(PartsProvider::GetDisplay()){
    // nothing to do
}

void DisplayController::onInit(){
    // display already initialized in parts provider
}

void DisplayController::onStart(){
    // nothing to do
}

void DisplayController::onTick(){
    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){

        const Heatpad& heatpad = PartsProvider::GetHeatpad();
        const Fan& fan = PartsProvider::GetFan();
        LvglObjects& lv = mDisplay.getLvglHandler().getObjects();

        lv.setFanState(FanStateToString(fan.getState()));
        lv.setFanTargetSpeed(fan.getTargetSpeed());
        lv.setFanMeasuredRpm(fan.getMeasuredRpm());
        
        lv.setHeatpadState(HeatpadStateToString(heatpad.getState()));
        lv.setHeatpadDuty(heatpad.getCurrentDutyCycle());
        lv.setHeatpadVoltage(heatpad.getMeasuredVoltage());
        lv.setHeatpadCurrent(heatpad.getMeasuredCurrent());

        mDisplay.giveRenderTrigger();
    }
}

} // namespace