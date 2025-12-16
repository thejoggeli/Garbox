#include "FermentationBehaviour.h"

#include "app/runtime/SnapshotRegistry.h"
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
    mHeaterEngine.reset();
    mHeaterEngine.setRegulationEnabled(true);
    mFirstTick = true;
}

void FermentationBehaviour::onBecomeDisabled(){
    mHeaterEngine.reset();
    sendFanCommand(false, 0.0f);
    sendHeatpadCommand(false, 0.0f, 5000_ms);
    sendFermentationStatus();
}

void FermentationBehaviour::onLogicTick(){

    const bool temperatureStatusValid = checkTemperatureStatus(SnapshotRegistry::GetTemperatureStatus());

    // set control engine input
    HeaterEngine::Input& input = mHeaterEngine.getInput();
    input.fanMeasuredRpm = SnapshotRegistry::GetFanSample().measuredRpm;
    input.fanStalled = SnapshotRegistry::GetFanStatus().state == FanState::Stalled;
    input.measuredHumidity = SnapshotRegistry::GetTemperatureSample().humidityRelative;
    input.measuredTemperature = SnapshotRegistry::GetTemperatureSample().temperatureCelcius;
    input.measuredHumidityValid = temperatureStatusValid;
    input.measuredTemperatureValid = temperatureStatusValid;
    input.targetTemperature = 30.0f;

    // perform control engine step
    mHeaterEngine.step();
    
    // send actuator commands
    const HeaterEngine::Output& output = mHeaterEngine.getOutput();
    sendHeatpadCommand(output.heaterEnabled, output.heaterPwmDuty, output.heaterPwmPeriodMicros);
    sendFanCommand(output.fanEnabled, output.fanTargetSpeed);
    sendFermentationStatus();

    // handle heartbeat received
    if(mHeartbeatReceived){
        // doFanTestStep();
        mHeartbeatReceived = false;
    }
    
    mFirstTick = false;
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

void FermentationBehaviour::sendFermentationStatus(){
    const HeaterEngine::Input& input = mHeaterEngine.getInput();
    if (!mFirstTick &&
        mLastFermentationStatus.heaterEngineState == mHeaterEngine.getState() &&
        mLastFermentationStatus.targetTemperature == input.targetTemperature){
        return;
    }
    FermentationStatusEvent event = makeFermentationStatusEvent();
    event->heaterEngineState = mHeaterEngine.getState();
    event->targetTemperature = input.targetTemperature;
    sendEvent(event);
    mLastFermentationStatus = *event.payload();
}

void FermentationBehaviour::onHeartbeat(const HeartbeatEvent& event){
    mHeartbeatReceived = true;
}

void FermentationBehaviour::onButtonStateChanged(const ButtonStateChangedEvent& event){
    if(event->newState == ButtonState::Released){

        static uint32_t s = 0;
        s = MathUtils::Wrap(s+1u, 3u);
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
            default: break;
        }

        // static uint32_t b = 4;
        // b = MathUtils::Wrap(b+1, 5u);
        // DisplayCommandEvent cmd = makeDisplayCommandEvent();
        // cmd->brightness = b/4.0f;
        // sendEvent(cmd);
    }
    else if(event->newState == ButtonState::PressedLong){
        mHeaterEngine.setRegulationEnabled(!mHeaterEngine.isRegulationEnabled());
    }
}
    
void FermentationBehaviour::onButtonRepeat(const ButtonRepeatEvent& event){
    if(event->count == 5){
        getHost()->requestChangeBehaviour(BehaviourId::Calibration);
    }
}
    
void FermentationBehaviour::onEncoderStep(const EncoderStepEvent& event){
    // nothing to do
}

bool FermentationBehaviour::checkTemperatureStatus(const TemperatureStatusPayload& payload){
    return payload.powerEnabled && payload.driverEnabled && !payload.resetting;
}

} // namespace
