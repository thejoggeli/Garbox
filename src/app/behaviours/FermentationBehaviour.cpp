#include "FermentationBehaviour.h"

#include "core/log/Log.h"
#include "core/time/TimeLiterals.h"

namespace Garbox {

FermentationBehaviour::FermentationBehaviour(const RuntimeContext& context): 
    // init members
    FermentationBehaviourAbs(context){
    // nothing to do
}

void FermentationBehaviour::onInit(){
    mControlEngine.init();
}

void FermentationBehaviour::onStart(){
    mControlEngine.reset();
}

void FermentationBehaviour::onBecomeActive(){
    mControlEngine.reset();
}

void FermentationBehaviour::onBecomeInactive(){
    // nothing to do
}

void FermentationBehaviour::onLogicTick(){

    // perform control engine step
    mControlEngine.step();

    // get control engine outputs
    const FermentationControlEngine::Outputs& outputs = mControlEngine.getOutputs();
    // TODO

    if(mHeartbeatReceived){
        constexpr static uint32_t SwitchStatesCount = 11;
        mSwitchState++;
        if(mSwitchState >= SwitchStatesCount){
            mSwitchState = 0;
        }
        applySwitchState();
        mHeartbeatReceived = false;
    }
}

void FermentationBehaviour::onHeartbeat(const EventRead<EventPayload::Heartbeat>& event){
    mHeartbeatReceived = true;
}

void FermentationBehaviour::onFanStatus(const EventRead<EventPayload::FanStatus>& event){
    LogDebug("FermentationBehaviour", "[FanStatus] state=%s, speed=%.1f%%",
        FanStateToString(event.payload->state),
        event.payload->targetSpeed * 100.0f
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.fanEnabled = (event.payload->state != FanState::Disabled);
    inputs.fanStalled = (event.payload->state == FanState::Stalled);
    inputs.fanTargetSpeed = event.payload->targetSpeed;
}

void FermentationBehaviour::onFanSample(const EventRead<EventPayload::FanSample>& event){
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.fanMeasuredRpm = event.payload->measuredRpm;
}

void FermentationBehaviour::onHeatpadStatus(const EventRead<EventPayload::HeatpadStatus>& event){
    LogDebug("FermentationBehaviour", "[HeatpadStatus] state=%s, duty=%.1f%%, period=%" PRIi32 "ms",
        HeatpadStateToString(event.payload->state),
        event.payload->dutyCycle,
        event.payload->periodMicros / 1000
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.heatpadEnabled = (event.payload->state != HeatpadState::Disabled);
    inputs.heatpadPwmDuty = event.payload->dutyCycle;
    inputs.heatpadPwmPeriodMicros = event.payload->periodMicros; 
}

void FermentationBehaviour::onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event){
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.temperatureEnabled = event.payload->powerEnabled && event.payload->driverEnabled;
    inputs.temperatureError = !inputs.temperatureEnabled && !event.payload->resetting; 
    LogDebug("FermentationBehaviour", "[TemperatureStatus] power=%u, driver=%u, reset=%u, error=%u",
        event.payload->powerEnabled, event.payload->driverEnabled, event.payload->resetting, inputs.temperatureError
    );
}

void FermentationBehaviour::onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event){
    LogDebug("FermentationBehaviour", "[TemperatureSample] temp=%.2f°C, hum=%.2f%%",
        event.payload->temperatureCelcius,
        event.payload->humidityRelative
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.temperatureCelcius = event.payload->temperatureCelcius;
    inputs.humidityRelative = event.payload->humidityRelative;
}

void FermentationBehaviour::onButtonStateChanged(const EventRead<EventPayload::ButtonStateChanged>& event){
    // nothing to do
}
    
void FermentationBehaviour::onButtonRepeat(const EventRead<EventPayload::ButtonRepeat>& event){
    // nothing to do
}
    
void FermentationBehaviour::onEncoderStep(const EventRead<EventPayload::EncoderStep>& event){
    // nothing to do
}

void FermentationBehaviour::applySwitchState(){
    switch(mSwitchState){
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
        TriggerDebug("FanController", "unhandled fan state", mSwitchState);
    }
}

void FermentationBehaviour::sendFanCommand(bool enabled, float speed){
    EventWrite event = makeEvent<EventPayload::FanCommand>();
    event.payload->enabled = enabled;
    event.payload->targetSpeed = speed;
    sendEvent(event.header);
}

} // namespace
