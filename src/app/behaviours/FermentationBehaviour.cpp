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
    mControlEngine.init();
}

void FermentationBehaviour::onStart(){
    BacklightCommandEvent cmd = makeBacklightCommandEvent();
    cmd->brightness = 0.5f;
    sendEvent(cmd);
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
    [[maybe_unused]] const FermentationControlEngine::Outputs& outputs = mControlEngine.getOutputs();
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

void FermentationBehaviour::onHeartbeat(const HeartbeatEvent& event){
    mHeartbeatReceived = true;
}

void FermentationBehaviour::onFanStatus(const FanStatusEvent& event){
    LogDebug("FermentationBehaviour", "[FanStatus] state=%s, speed=%.1f%%",
        FanStateToString(event->state),
        event->targetSpeed * 100.0f
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.fanEnabled = (event->state != FanState::Disabled);
    inputs.fanStalled = (event->state == FanState::Stalled);
    inputs.fanTargetSpeed = event->targetSpeed;
}

void FermentationBehaviour::onFanSample(const FanSampleEvent& event){
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.fanMeasuredRpm = event->measuredRpm;
}

void FermentationBehaviour::onHeatpadStatus(const HeatpadStatusEvent& event){
    LogDebug("FermentationBehaviour", "[HeatpadStatus] state=%s, duty=%.1f%%, period=%" PRIi32 "ms",
        HeatpadStateToString(event->state),
        event->dutyCycle * 100.0f,
        event->periodMicros / 1000
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.heatpadEnabled = event->state != HeatpadState::Disabled;
    inputs.heatpadPwmDuty = event->dutyCycle;
    inputs.heatpadPwmPeriodMicros = event->periodMicros; 
}

void FermentationBehaviour::onTemperatureStatus(const TemperatureStatusEvent& event){
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.temperatureEnabled = event->powerEnabled && event->driverEnabled;
    inputs.temperatureError = !inputs.temperatureEnabled && !event->resetting; 
    LogDebug("FermentationBehaviour", "[TemperatureStatus] power=%u, driver=%u, reset=%u, error=%u",
        event->powerEnabled, event->driverEnabled, event->resetting, inputs.temperatureError
    );
}

void FermentationBehaviour::onTemperatureSample(const TemperatureSampleEvent& event){
    LogDebug("FermentationBehaviour", "[TemperatureSample] temp=%.2f°C, hum=%.2f%%",
        event->temperatureCelcius,
        event->humidityRelative
    );
    FermentationControlEngine::Inputs& inputs = mControlEngine.getInputs();
    inputs.temperatureCelcius = event->temperatureCelcius;
    inputs.humidityRelative = event->humidityRelative;
}

void FermentationBehaviour::onButtonStateChanged(const ButtonStateChangedEvent& event){
    if(event->newState == ButtonState::Released){
        static uint32_t b = 4;
        b = MathUtils::Wrap(b+1, 5u);
        BacklightCommandEvent cmd = makeBacklightCommandEvent();
        cmd->brightness = b/4.0f;
        sendEvent(cmd);
    }
}
    
void FermentationBehaviour::onButtonRepeat(const ButtonRepeatEvent& event){
    // nothing to do
}
    
void FermentationBehaviour::onEncoderStep(const EncoderStepEvent& event){
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
    FanCommandEvent event = makeFanCommandEvent();
    event->enabled = enabled;
    event->targetSpeed = speed;
    sendEvent(event);
}

} // namespace
