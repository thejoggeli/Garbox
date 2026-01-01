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
    states().fermentationStatus.setTargetTemperature(32.0f);
    cmd->brightness = 0.5f;
    sendEvent(cmd);
}

void FermentationBehaviour::onBecomeEnabled(){
    mFermentationEngine.reset();
    states().fermentationStatus.setPowerOn(true);
    states().fermentationStatus.setFanAuto(true);
    mFirstTick = true;
}

void FermentationBehaviour::onBecomeDisabled(){
    mFermentationEngine.reset();
    sendFanCommand(false, 0.0f);
    sendHeatpadCommand(false, 0.0f, 5000_ms);
    updateFermentationStatus();
}

void FermentationBehaviour::onLogicTick(){
    
    const bool temperatureStatusValid = checkTemperatureStatus();

    // set control engine input
    FermentationEngine::Input& input = mFermentationEngine.getInput();
    input.targetTemperature          = states().fermentationStatus.getTargetTemperature();
    input.regulationEnabled          = states().fermentationStatus.getPowerOn();
    input.fanMeasuredRpm             = states().fanSample.getMeasuredRpm();
    input.fanStalled                 = states().fanStatus.getState() == FanState::Stalled;
    input.measuredHumidity           = states().temperatureSample.getHumidityRelative();
    input.measuredTemperature        = states().temperatureSample.getTemperatureCelcius();
    input.measuredHumidityValid      = temperatureStatusValid;
    input.measuredTemperatureValid   = temperatureStatusValid;

    // perform control engine step
    mFermentationEngine.step();
    
    // send actuator commands
    const FermentationEngine::Output& output = mFermentationEngine.getOutput();
    const bool autoFan = states().fermentationStatus.getFanAuto();
    sendHeatpadCommand(output.heaterEnabled, output.heaterPwmDuty, output.heaterPwmPeriodMicros);
    sendFanCommand(output.fanEnabled && autoFan, output.fanTargetSpeed);
    updateFermentationStatus();
    
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

void FermentationBehaviour::updateFermentationStatus(){
    FermentationStatusState& status = states().fermentationStatus;
    const FermentationEngine::Input& input = mFermentationEngine.getInput();
    status.setState(mFermentationEngine.getState());
    status.setTargetTemperature(input.targetTemperature);
}

void FermentationBehaviour::onHeartbeatEvent(const HeartbeatEvent& event){
    // nothing to do
}

void FermentationBehaviour::onRequestFermentationModeEvent(const RequestFermentationModeEvent& event){
    states().fermentationStatus.setPowerOn(event->enabled);
}

void FermentationBehaviour::onRequestTargetTemperatureEvent(const RequestTargetTemperatureEvent& event){
    states().fermentationStatus.setTargetTemperature(event->targetTemperature);
}

void FermentationBehaviour::onRequestFanModeEvent(const RequestFanModeEvent& event){
    states().fermentationStatus.setFanAuto(event->autoFan);
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
