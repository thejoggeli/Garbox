#include "FanController.h"

#include "app/parts/StatusLeds.h"
#include "app/providers/PartsProvider.h"
#include "app/providers/PiezoSequences.h"
#include "assert/Assert.h"
#include "core/log/Log.h"
#include "parts/piezo/PiezoPlayer.h"

namespace Garbox {

constexpr static uint32_t SwitchStatesCount = 11;

FanController::FanController(): 
    // init members
    ControllerAbs(ControllerId::Fan),
    mFan(PartsProvider::GetFan()),
    mStatusLed(PartsProvider::GetStatusLed(StatusLedId::Custom1)){
    // nothing to do
}

void FanController::onInit(){
    // fan already initialized in parts provider
    // fan state changed
    mFan.setStateChangedCallback([this](FanState oldState, FanState newState){
        handleFanStateChanged(oldState, newState);
    });
    // fan stalled alert
    mFan.setStalledAlertCallback([this](uint32_t counter){
        handleFanStalledAlert(counter);
    });
}

void FanController::onStart(){
    mFan.start();
    applySwitchState();
}

void FanController::onTick(){
    // fan tick
    mFan.tick();
}

void FanController::onFanCommand(const EventView<EventData::FanCommand> event){
    mApplyingCommand = true;
    bool changed = false;
    // apply enabled
    if(mFan.isEnabled() != event.data->enabled){
        mFan.setEnabled(event.data->enabled);
        changed = true;
    }
    // apply speed
    if(mFan.getTargetSpeed() != event.data->targetSpeed){
        mFan.setTargetSpeed(event.data->targetSpeed);
        changed = true;
    }
    mApplyingCommand = false;
    // send status 
    if(changed){
        sendStatusEvent();
    }
}

void FanController::onHeartbeat(const EventView<EventData::Heartbeat>& event){
    if(++mSwitchState >= SwitchStatesCount){
        mSwitchState = 0;
    }
    applySwitchState();
}

void FanController::applySwitchState(){
    switch(mSwitchState){
    case 0:
        mFan.setEnabled(false);
        mStatusLed.setBrightnessSmooth(0.0f, 600_ms);
        sendStatusEvent();
        break;
    case 1: 
        // stay
        break;
    case 2:
        mFan.setEnabled(true);
        mFan.setTargetSpeed(0.4f);
        mStatusLed.setBrightnessSmooth(0.25f, 1000_ms);
        sendStatusEvent();
        break;
    case 3:
        mFan.setTargetSpeed(0.6f);
        mStatusLed.setBrightnessSmooth(0.5f, 1000_ms);
        sendStatusEvent();
        break;
    case 4:
        mFan.setTargetSpeed(0.8f);
        mStatusLed.setBrightnessSmooth(0.75f, 1000_ms);
        sendStatusEvent();
        break;
    case 5:
        mFan.setTargetSpeed(1.0f);
        mStatusLed.setBrightnessSmooth(1.0f, 1000_ms);
        sendStatusEvent();
        break;
    case 6:
    case 7:
        // stay
        break;
    case 8:
        mFan.setTargetSpeed(0.8f);
        mStatusLed.setBrightnessSmooth(0.75f, 1000_ms);
        sendStatusEvent();
        break;
    case 9: 
        mFan.setTargetSpeed(0.6f);
        mStatusLed.setBrightnessSmooth(0.5f, 1000_ms);
        sendStatusEvent();
        break;
    case 10:
        mFan.setTargetSpeed(0.4f);
        mStatusLed.setBrightnessSmooth(0.25f, 1000_ms);
        sendStatusEvent();
        break;
    default:
        TriggerDebug("FanController", "unhandled fan state", mSwitchState);
    }
}

void FanController::handleFanStateChanged(FanState oldState, FanState newState){
    LogDebug("GarboxController", "fan state changed: %s => %s", 
        FanStateToString(oldState), 
        FanStateToString(newState)
    );
    // send fan status event
    sendStatusEvent();
}

void FanController::handleFanStalledAlert(uint32_t counter){
    PiezoPlayer& piezoPlayer = PartsProvider::GetPiezoPlayer();
    piezoPlayer.playSequence(PiezoSequences::GetFanStalled());
}

void FanController::sendStatusEvent(){
    if(mApplyingCommand){
        return;
    }
    EventWrapper wrapper = getEventFactory().make<EventData::FanStatus>();
    wrapper.data->state = mFan.getState();
    wrapper.data->targetSpeed = mFan.getTargetSpeed();
    wrapper.data->measuredRpm = mFan.getMeasuredRpm();
    sendEvent(wrapper.event);
}

} // namespace