#include "FanController.h"

#include "app/parts/StatusLeds.h"
#include "app/providers/PartsProvider.h"
#include "app/providers/PiezoSequences.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "modules/parts/piezo/PiezoPlayer.h"

namespace Garbox {

constexpr static uint32_t SwitchStatesCount = 11;

FanController::FanController(ControllerId id): 
    // init members
    ControllerAbs(id),
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

void FanController::onInputTick(){
    // updates MeasuredRpm + FanState
    // may trigger handleFanStateChanged()
    mFan.tick();
    if(mFanStateChanged){
        sendStatusEvent();
        mFanStateChanged = false;
    }
}

void FanController::onOutputTick(){
    if(mSwitchStateChanged){
        applySwitchState();
        sendStatusEvent();
        mSwitchStateChanged = false;
    }
}

void FanController::onFanCommand(const EventView<EventData::FanCommand> event){
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
    // send status 
    if(changed){
        sendStatusEvent();
    }
}

void FanController::onHeartbeat(const EventView<EventData::Heartbeat>& event){
    mSwitchState++;
    if(mSwitchState >= SwitchStatesCount){
        mSwitchState = 0;
    }
    mSwitchStateChanged = true;
}

void FanController::applySwitchState(){
    switch(mSwitchState){
    case 0:
        mFan.setEnabled(false);
        mStatusLed.setBrightnessSmooth(0.0f, 600_ms);
        break;
    case 1: 
        // stay
        break;
    case 2:
        mFan.setEnabled(true);
        mFan.setTargetSpeed(0.4f);
        mStatusLed.setBrightnessSmooth(0.25f, 1000_ms);
        break;
    case 3:
        mFan.setTargetSpeed(0.6f);
        mStatusLed.setBrightnessSmooth(0.5f, 1000_ms);
        break;
    case 4:
        mFan.setTargetSpeed(0.8f);
        mStatusLed.setBrightnessSmooth(0.75f, 1000_ms);
        break;
    case 5:
        mFan.setTargetSpeed(1.0f);
        mStatusLed.setBrightnessSmooth(1.0f, 1000_ms);
        break;
    case 6:
    case 7:
        // stay
        break;
    case 8:
        mFan.setTargetSpeed(0.8f);
        mStatusLed.setBrightnessSmooth(0.75f, 1000_ms);
        break;
    case 9: 
        mFan.setTargetSpeed(0.6f);
        mStatusLed.setBrightnessSmooth(0.5f, 1000_ms);
        break;
    case 10:
        mFan.setTargetSpeed(0.4f);
        mStatusLed.setBrightnessSmooth(0.25f, 1000_ms);
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
    mFanStateChanged = true;
}

void FanController::handleFanStalledAlert(uint32_t counter){
    PiezoPlayer& piezoPlayer = PartsProvider::GetPiezoPlayer();
    piezoPlayer.playSequence(PiezoSequences::GetFanStalled());
}

void FanController::sendStatusEvent(){
    EventWrapper wrapper = getEventFactory().make<EventData::FanStatus>();
    wrapper.data->state = mFan.getState();
    wrapper.data->targetSpeed = mFan.getTargetSpeed();
    wrapper.data->measuredRpm = mFan.getMeasuredRpm();
    sendEvent(wrapper.event);
}

} // namespace