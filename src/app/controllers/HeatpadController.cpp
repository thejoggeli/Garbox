#include "HeatpadController.h"

#include "app/providers/ColorMaps.h"
#include "app/providers/PartsProvider.h"
#include "parts/heatpad/Heatpad.h"
#include "parts/led/rgb/RgbLed.h"

namespace Garbox {

HeatpadController::HeatpadController(): 
    // init members
    ControllerAbs(ControllerId::Heatpad),
    mHeatpad(PartsProvider::GetHeatpad()),
    mLed(PartsProvider::GetRgbLed()){
    // nothing to do
}

void HeatpadController::onInit(){
    // heatpad already initialized in parts provider
    mHeatpad.setStateChangedHandler([this](HeatpadState oldState, HeatpadState newState){
        handleHeatpadStateChanged(oldState, newState);
    });
}

void HeatpadController::onStart(){
    mHeatpad.setDutyCycle(0.5f);
    mHeatpad.setPeriodDurationMicros(5000_ms);
    mHeatpad.setEnabled(true);
}

void HeatpadController::onTick(){
    mHeatpad.tick();

    // rgb led tick
    static const ColorMap& colorMap = ColorMaps::GetRedBlue();
    constexpr float brightness = 0.14f;
    float const tColorMap = mHeatpad.getMeasuredVoltage() / 17.0f;
    HslColor hslColor = colorMap.interpolateHsl(tColorMap);
    hslColor.l = brightness;
    mLed.setColor(hslColor.toLinearRgb());
}

void HeatpadController::onHeatpadCommand(const EventView<EventData::HeatpadCommand> event){
    mApplyingCommand = true;
    bool changed = false;
    // apply enabled 
    if(mHeatpad.isEnabled() != event.data->enabled){
        mHeatpad.setEnabled(event.data->enabled);
        changed = true;
    }
    // apply duty cycle
    if(mHeatpad.getNextDutyCycle() != event.data->dutyCycle){
        mHeatpad.setDutyCycle(event.data->dutyCycle);
        changed = true;
    }
    // apply period
    if(mHeatpad.getNextPeriodDurationMicros() != event.data->periodMicros){
        mHeatpad.setPeriodDurationMicros(event.data->periodMicros);
        changed = true;
    }
    mApplyingCommand = false;
    // send status event
    if(changed){
        sendStatusEvent();
    }
}

void HeatpadController::handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState){
    sendStatusEvent();
}

void HeatpadController::sendStatusEvent(){
    if(mApplyingCommand){
        return;
    }
    EventWrapper wrapper = getEventFactory().make<EventData::HeatpadStatus>();
    wrapper.data->state = mHeatpad.getState();
    sendEvent(wrapper.event);
}

} // namespace