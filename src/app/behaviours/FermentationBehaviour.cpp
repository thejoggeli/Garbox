#include "FermentationBehaviour.h"

#include "core/log/Log.h"
#include "core/time/TimeLiterals.h"

namespace Garbox {

FermentationBehaviour::FermentationBehaviour(ComponentId id) : AppBehaviourAbs(id) {
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

void FermentationBehaviour::onHeartbeat(const EventView<EventData::Heartbeat>& event){
    mHeartbeatReceived = true;
}

void FermentationBehaviour::onFanStatus(const EventView<EventData::FanStatus>& event){
    LogDebug("FermentationBehaviour", "[FanStatus] state=%s, speed=%.1f%%, rpm=%" PRIi32,
        FanStateToString(event.data->state),
        event.data->targetSpeed * 100.0f,
        static_cast<uint32_t>(event.data->measuredRpm + 0.5f)
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.fanEnabled = (event.data->state != FanState::Disabled);
    inputs.fanStalled = (event.data->state == FanState::Stalled);
    inputs.fanTargetSpeed = event.data->targetSpeed;
    inputs.fanMeasuredRpm = event.data->measuredRpm;
}

void FermentationBehaviour::onHeatpadStatus(const EventView<EventData::HeatpadStatus>& event){
    LogDebug("FermentationBehaviour", "[HeatpadStatus] state=%s, duty=%.1f%%, period=%" PRIi32 "ms",
        HeatpadStateToString(event.data->state),
        event.data->duty,
        event.data->periodMicros / 1000
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.heatpadEnabled = (event.data->state != HeatpadState::Disabled);
    inputs.heatpadPwmDuty = event.data->duty;
    inputs.heatpadPwmPeriodMicros = event.data->periodMicros; 
}

void FermentationBehaviour::onTemperatureStatus(const EventView<EventData::TemperatureStatus>& event){
    LogDebug("FermentationBehaviour", "[TemperatureStatus] en=%u, err=%u, temp=%.2f°C, hum=%.2f%%",
        event.data->sensorEnabled,
        event.data->sensorError,
        event.data->temperatureCelcius,
        event.data->humidityRelative
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.temperatureEnabled = event.data->sensorEnabled;
    inputs.temperatureError = event.data->sensorError;
    inputs.temperatureCelcius = event.data->temperatureCelcius;
    inputs.humidityRelative = event.data->humidityRelative;
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
    EventWrapper wrapper = makeEvent<EventData::FanCommand>();
    wrapper.data->enabled = enabled;
    wrapper.data->targetSpeed = speed;
    sendEvent(wrapper.event);
}

} // namespace
