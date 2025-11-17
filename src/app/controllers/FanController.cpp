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
    mFan.setStateChangedCallback([this](Fan::State oldState, Fan::State newState){
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

    // print fan rpm
    if(mRpmTimer.isExpired() || mRpmTimer.isReset()){
        static float lastRpmValue = 0;
        float const rpmValue = mFan.getMeasuredRpm();
        float const rpmDiff = std::fabs(rpmValue - lastRpmValue);
        if(rpmDiff > 0.0f){ 
            LogDebug("FanController", "Measured RPM: %.0f", rpmValue);
            lastRpmValue = rpmValue;
        }
        mRpmTimer.start(200_ms);
    }
}

void FanController::onFanEvent(const EventView<FanEventData>& event){
    EventWrapper wrapper = getEventFactory().make<FanEventData>();
    wrapper.data->measuredRpm = 1234.5f;
    wrapper.data->targetSpeed = 50.0f;
    sendEvent(wrapper.event);
}

void FanController::onHeartbeatEvent(const EventView<HeartbeatEventData>& event){
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
        break;
    case 1: 
        // stay
        break;
    case 2:
        mFan.setEnabled(true);
        mFan.setSpeed(0.4f);
        mStatusLed.setBrightnessSmooth(0.25f, 1000_ms);
        break;
    case 3:
        mFan.setSpeed(0.6f);
        mStatusLed.setBrightnessSmooth(0.5f, 1000_ms);
        break;
    case 4:
        mFan.setSpeed(0.8f);
        mStatusLed.setBrightnessSmooth(0.75f, 1000_ms);
        break;
    case 5:
        mFan.setSpeed(1.0f);
        mStatusLed.setBrightnessSmooth(1.0f, 1000_ms);
        break;
    case 6:
    case 7:
        // stay
        break;
    case 8:
        mFan.setSpeed(0.8f);
        mStatusLed.setBrightnessSmooth(0.75f, 1000_ms);
        break;
    case 9: 
        mFan.setSpeed(0.6f);
        mStatusLed.setBrightnessSmooth(0.5f, 1000_ms);
        break;
    case 10:
        mFan.setSpeed(0.4f);
        mStatusLed.setBrightnessSmooth(0.25f, 1000_ms);
        break;
    default:
        TriggerDebug("FanController", "unhandled fan state", mSwitchState);
    }
}

void FanController::handleFanStateChanged(Fan::State oldState, Fan::State newState){
    LogDebug("GarboxController", "fan state changed: %s => %s", 
        Fan::StateToString(oldState), 
        Fan::StateToString(newState)
    );
}

void FanController::handleFanStalledAlert(uint32_t counter){
    PiezoPlayer& piezoPlayer = PartsProvider::GetPiezoPlayer();
    piezoPlayer.playSequence(PiezoSequences::GetFanStalled());
}

} // namespace