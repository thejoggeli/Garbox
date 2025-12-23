#include "FermentationBehaviour.h"

#include "core/log/Log.h"
#include "core/time/TimeLiterals.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

FermentationBehaviour::FermentationBehaviour(): 
    // init members
    FermentationBehaviourAbs(){
    // nothing to do
}

void FermentationBehaviour::onInit(){
    // nothing to do
}

void FermentationBehaviour::onStart(){
    DisplayCommandEvent cmd = makeDisplayCommandEvent();
    cmd->brightness = 0.5f;
    sendEvent(cmd);
}

void FermentationBehaviour::onBecomeEnabled(){
    mFermentationEngine.reset();
    mFermentationEngine.setRegulationEnabled(true);
    mFirstTick = true;
    mStepTimer.reset();
}

void FermentationBehaviour::onBecomeDisabled(){
    mFermentationEngine.reset();
    sendFanCommand(false, 0.0f);
    sendHeatpadCommand(false, 0.0f, 5000_ms);
    updateFermentationStatus();
    mStepTimer.reset();
}

void FermentationBehaviour::onLogicTick(){

    if(mFirstTick || mStepTimer.isExpired()){
    
        const bool temperatureStatusValid = checkTemperatureStatus();

        // set control engine input
        FermentationEngine::Input& input = mFermentationEngine.getInput();
        input.fanMeasuredRpm             = states().fanSample.getMeasuredRpm();
        input.fanStalled                 = states().fanStatus.getState() == FanState::Stalled;
        input.measuredHumidity           = states().temperatureSample.getHumidityRelative();
        input.measuredTemperature        = states().temperatureSample.getTemperatureCelcius();
        input.measuredHumidityValid      = temperatureStatusValid;
        input.measuredTemperatureValid   = temperatureStatusValid;

        static uint32_t count = 0;
        static uint32_t target = 30;
        if(count++ >= 10*10){
            count = 0;
            target = (target == 30) ? 35 : 30;
        }
        input.targetTemperature = target;

        // perform control engine step
        mFermentationEngine.step();
        
        // send actuator commands
        const FermentationEngine::Output& output = mFermentationEngine.getOutput();
        sendHeatpadCommand(output.heaterEnabled, output.heaterPwmDuty, output.heaterPwmPeriodMicros);
        sendFanCommand(output.fanEnabled, output.fanTargetSpeed);
        updateFermentationStatus();
        
        mStepTimer.restart(100_ms); // 10 Hz
        mFirstTick = false;
    }
}

void FermentationBehaviour::sendHeatpadCommand(bool enabled, float dutyCycle, uint32_t periodMicros){
    if (!mFirstTick &&
        mLastHeatpadCommand.enabled == enabled &&
        mLastHeatpadCommand.dutyCycle == dutyCycle &&
        mLastHeatpadCommand.periodMicros == periodMicros){
        return;
    }
    HeatpadCommandEvent event = makeHeatpadCommandEvent();
    event->enabled = enabled;
    event->dutyCycle = dutyCycle;
    event->periodMicros = periodMicros;
    sendEvent(event);
    mLastHeatpadCommand = *event.payload();
}

void FermentationBehaviour::sendFanCommand(bool enabled, float speed){
    if (!mFirstTick && mLastFanCommand.enabled == enabled && mLastFanCommand.targetSpeed == speed){
        return;
    }
    FanCommandEvent event = makeFanCommandEvent();
    event->enabled = enabled;
    event->targetSpeed = speed;
    sendEvent(event);
    mLastFanCommand = *event.payload();
}

void FermentationBehaviour::updateFermentationStatus(){
    FermentationStatusState& status = states().fermentationStatus;
    const FermentationEngine::Input& input = mFermentationEngine.getInput();
    status.setState(mFermentationEngine.getState());
    status.setTargetTemperature(input.targetTemperature);
}

void FermentationBehaviour::onHeartbeatEvent(const HeartbeatEvent& event){
    // nothing to do
}

void FermentationBehaviour::onButtonEvent(const ButtonEvent& event){
    if(event->newState == ButtonState::Released){

        static uint32_t s = 0;
        s = MathUtils::Wrap(s+1u, 4u);
        switch(s){
            case 0:
                getHost()->requestChangeScreen(ScreenId::Main);
                break;
            case 1:
                getHost()->requestChangeScreen(ScreenId::Debug);
                break;
            case 2:
                getHost()->requestChangeScreen(ScreenId::EventLog);
                break;
            case 3:
                getHost()->requestChangeScreen(ScreenId::StateLog);
                break;
            default: break;
        }

        static uint32_t b = 4;
        b = MathUtils::Wrap(b+1, 4u) + 1;
        DisplayCommandEvent cmd = makeDisplayCommandEvent();
        cmd->brightness = b/4.0f;
        sendEvent(cmd);
    }
    else if(event->newState == ButtonState::PressedLong){
        mFermentationEngine.setRegulationEnabled(!mFermentationEngine.isRegulationEnabled());
    }
}
    
void FermentationBehaviour::onButtonRepeatEvent(const ButtonRepeatEvent& event){
    if(event->count == 5){
        getHost()->requestChangeBehaviour(BehaviourId::Calibration);
    }
}
    
void FermentationBehaviour::onEncoderStepEvent(const EncoderStepEvent& event){
    // nothing to do
}

bool FermentationBehaviour::checkTemperatureStatus(){
    const TemperatureStatusState& status = states().temperatureStatus;
    return status.getPowerEnabled() && status.getDriverEnabled() && !status.getResetting();
}

void FermentationBehaviour::onFanSampleStateChanged(const FanSampleState& state){
    // nothing to do
}

void FermentationBehaviour::onFanStatusStateChanged(const FanStatusState& state){
    // nothing to do
}

void FermentationBehaviour::onTemperatureSampleStateChanged(const TemperatureSampleState& state){
    // nothing to do
}

void FermentationBehaviour::onTemperatureStatusStateChanged(const TemperatureStatusState& state){
    // nothing to do
}

} // namespace
