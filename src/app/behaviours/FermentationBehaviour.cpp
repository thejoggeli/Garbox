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
    mHeaterEngine.reset();
    mHeaterEngine.setRegulationEnabled(true);
    mFirstTick = true;
}

void FermentationBehaviour::onBecomeDisabled(){
    mHeaterEngine.reset();
    sendDisableActuatorsCommands();
}

void FermentationBehaviour::onLogicTick(){

    // set control engine input
    HeaterEngine::Input& input = mHeaterEngine.getInput();
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

void FermentationBehaviour::sendDisableActuatorsCommands(){
    sendFanCommand(false, 0.0f);
    sendHeatpadCommand(false, 0.0f, 5000_ms);
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
    mLastHeatpadCommand.enabled = enabled;
    mLastHeatpadCommand.dutyCycle = dutyCycle;
    mLastHeatpadCommand.periodMicros = periodMicros;
}

void FermentationBehaviour::sendFanCommand(bool enabled, float speed){
    if (!mFirstTick && mLastFanCommand.enabled == enabled && mLastFanCommand.targetSpeed == speed){
        return;
    }
    FanCommandEvent event = makeFanCommandEvent();
    event->enabled = enabled;
    event->targetSpeed = speed;
    sendEvent(event);
    mLastFanCommand.enabled = enabled;
    mLastFanCommand.targetSpeed = speed;
}

void FermentationBehaviour::sendFermentationStatus(){
    const HeaterEngine::Input& input = mHeaterEngine.getInput();
    if (!mFirstTick &&
        mLastFermentationStatus.heaterEngineState == mHeaterEngine.getState() &&
        mLastFermentationStatus.targetTemperature == input.targetTemperature &&
        mLastFermentationStatus.measuredTemperature == input.measuredTemperature &&
        mLastFermentationStatus.measuredHumidity == input.measuredHumidity){
        return;
    }
    FermentationStatusEvent event = makeFermentationStatusEvent();
    event->heaterEngineState = mHeaterEngine.getState();
    event->targetTemperature = input.targetTemperature;
    event->measuredTemperature = input.measuredTemperature;
    event->measuredHumidity = input.measuredHumidity;
    sendEvent(event);
    mLastFermentationStatus.heaterEngineState = event->heaterEngineState;
    mLastFermentationStatus.targetTemperature = event->targetTemperature;
    mLastFermentationStatus.measuredTemperature = event->measuredTemperature;
    mLastFermentationStatus.measuredHumidity = event->measuredHumidity;
}

void FermentationBehaviour::onHeartbeat(const HeartbeatEvent& event){
    mHeartbeatReceived = true;
}

void FermentationBehaviour::onFanStatus(const FanStatusEvent& event){
    HeaterEngine::Input& input = mHeaterEngine.getInput();
    input.fanStalled = (event->state == FanState::Stalled);
}

void FermentationBehaviour::onFanSample(const FanSampleEvent& event){
    HeaterEngine::Input& input = mHeaterEngine.getInput();
    input.fanMeasuredRpm = event->measuredRpm;
}

void FermentationBehaviour::onHeatpadStatus(const HeatpadStatusEvent& event){
    // nothing to do
}

void FermentationBehaviour::onTemperatureStatus(const TemperatureStatusEvent& event){
    HeaterEngine::Input& input = mHeaterEngine.getInput();
    const bool enabled = event->powerEnabled && event->driverEnabled && !event->resetting;
    if(!enabled){
        input.measuredTemperatureValid = false;
        input.measuredHumidityValid = false;
    }
}

void FermentationBehaviour::onTemperatureSample(const TemperatureSampleEvent& event){
    HeaterEngine::Input& input = mHeaterEngine.getInput();
    input.measuredTemperature = event->temperatureCelcius;
    input.measuredHumidity = event->humidityRelative;
    input.measuredTemperatureValid = true;
    input.measuredHumidityValid = true;
}

void FermentationBehaviour::onButtonStateChanged(const ButtonStateChangedEvent& event){
    if(event->newState == ButtonState::Released){

        static uint32_t s = 0;
        s = MathUtils::Wrap(s+1u, 2u);
        switch(s){
            case 0:
                getHost()->requestChangeScreen(ScreenId::Main);
                break;
            case 1:
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

void FermentationBehaviour::doFanTestStep(){

    constexpr static uint32_t SwitchStatesCount = 11;
    static uint32_t switchState = 0;
    switchState = MathUtils::Wrap(switchState+1u, SwitchStatesCount);

    switch(switchState){
    case  0: sendFanCommand(false, 0.0f); break;
    case  1: break; // stay
    case  2: sendFanCommand(true, 0.4f); break;
    case  3: sendFanCommand(true, 0.6f); break;
    case  4: sendFanCommand(true, 0.8f); break;
    case  5: sendFanCommand(true, 1.0f); break;
    case  6: break; // stay
    case  7: break; // stay
    case  8: sendFanCommand(true, 0.8f); break;
    case  9: sendFanCommand(true, 0.6f); break;
    case 10: sendFanCommand(true, 0.4f); break;
    default:
        TriggerDebug("FanController", "unhandled fan state", switchState);
    }
}

} // namespace
