#include "FanController.h"

#include "app/providers/PartsProvider.h"
#include "app/providers/PiezoSequences.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "core/util/math/MathUtils.h"
#include "modules/parts/led/single/AnimatedLed.h"
#include "modules/parts/piezo/PiezoPlayer.h"

namespace Garbox {

static constexpr float PidKp = 0.005f; 
static constexpr float PidKi = 0.01f;
static constexpr float PidKd = 0.0f;
static constexpr float PidOutputMin = 0.0f;
static constexpr float PidOutputMax = 1.0;

FanController::FanController(): 
    // init members
    FanControllerAbs(),
    mFan(PartsProvider::GetFan()),
    mStatusLed(PartsProvider::GetStatusLed(StatusLedId::Custom1)),
    mPid(PidKp, PidKi, PidKd, PidOutputMin, PidOutputMax){
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
    // updates MeasuredRpm and FanState
    mFan.tick();
    if(mStateChanged){
        sendStatusEvent();
        mStateChanged = false;
    }
    // send rpm sample if changed
    float measuredRpm = mFan.getMeasuredRpm();
    if(measuredRpm != mLastMeasuredRpm){
        sendSampleEvent(measuredRpm);
        mLastMeasuredRpm = measuredRpm;
    }
}

void FanController::onOutputTick(){
    if(mUsePid && mFan.isEnabled()){
        const bool filtered = false;
        const float measuredRpm = mFan.getMeasuredRpm(filtered);
        const float dt = Time::GetTickDeltaSeconds();
        float duty = mPid.step(measuredRpm, mTargetSpeed, dt);
        mFan.setTargetSpeed(duty);
    }
}

void FanController::onFanCommand(const FanCommandEvent& event){
    // apply enabled
    if(mFan.isEnabled() != event->enabled){
        mFan.setEnabled(event->enabled);
        if(!event->enabled){
            mPid.reset(); // reset pid when disabling fan
            mTargetSpeed = 0.0f;
        }
        mStateChanged = true;
    }
    // apply use pid
    if(event->enableRpmControl != mUsePid){
        mUsePid = event->enableRpmControl;
        if(!mUsePid){
            mPid.reset(); // reset pid when leaving pid mode 
        }
        mStateChanged = true;
    }
    // apply target speed only if fan enabled
    if(event->enabled && (event->targetSpeed != mTargetSpeed)){
        mTargetSpeed = event->targetSpeed;
        if(!mUsePid){
            mFan.setTargetSpeed(mTargetSpeed);
        }
        mStateChanged = true;
    }
    // set status led
    if(mStateChanged){
        if(mFan.isEnabled()){
            float brightness = MathUtils::Map(mTargetSpeed, 0.4f, 1.0f, 0.25f, 1.0f);
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
    FanStatusEvent event = makeFanStatusEvent();
    event->state = mFan.getState();
    event->targetSpeed = mTargetSpeed;
    event->rpmControlEnabled = mUsePid;
    sendEvent(event);
}

void FanController::sendSampleEvent(float measuredRpm){
    FanSampleEvent event = makeFanSampleEvent();
    event->measuredRpm = measuredRpm;
    sendEvent(event);
}

} // namespace