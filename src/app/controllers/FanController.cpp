#include "FanController.h"

#include "app/providers/PartsProvider.h"
#include "app/providers/PiezoSequences.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/util/math/MathUtils.h"
#include "modules/parts/led/single/AnimatedLed.h"
#include "modules/parts/piezo/PiezoPlayer.h"

namespace Garbox {

FanController::FanController(ComponentId id): 
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
}

void FanController::onInputTick(){
    // updates MeasuredRpm + FanState
    // may trigger handleFanStateChanged()
    mFan.tick();
    if(mStateChanged){
        sendStatusEvent();
        mStateChanged = false;
    }
}

void FanController::onOutputTick(){
    // nothing to do
}

void FanController::onFanCommand(const EventRead<EventData::FanCommand> event){
    // apply enabled
    if(mFan.isEnabled() != event.payload->enabled){
        mFan.setEnabled(event.payload->enabled);
        mStateChanged = true;
    }
    // apply speed
    if(mFan.getTargetSpeed() != event.payload->targetSpeed){
        mFan.setTargetSpeed(event.payload->targetSpeed);
        mStateChanged = true;
    }
    // send status 
    if(mStateChanged){
        if(mFan.isEnabled()){
            float brightness = MathUtils::Map(mFan.getTargetSpeed(), 0.4f, 1.0f, 0.25f, 1.0f);
            mStatusLed.setBrightnessSmooth(brightness, 500_ms);
        }
        else {
            mStatusLed.setBrightnessSmooth(0.0f, 500_ms);
        }
    }
}

void FanController::handleFanStateChanged(FanState oldState, FanState newState){
    mStateChanged = true;
}

void FanController::handleFanStalledAlert(uint32_t counter){
    PiezoPlayer& piezoPlayer = PartsProvider::GetPiezoPlayer();
    piezoPlayer.playSequence(PiezoSequences::GetFanStalled());
}

void FanController::sendStatusEvent(){
    EventWrite event = makeEvent<EventData::FanStatus>();
    event.payload->state = mFan.getState();
    event.payload->targetSpeed = mFan.getTargetSpeed();
    event.payload->measuredRpm = mFan.getMeasuredRpm();
    sendEvent(event.header);
}

} // namespace