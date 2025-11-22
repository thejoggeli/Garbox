// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "CalibrationBehaviourAbs.h"

namespace Garbox {

CalibrationBehaviourAbs::CalibrationBehaviourAbs(): 
    // init memberes
    BaseBehaviourAbs(ComponentId::CalibrationBehaviour, BehaviourId::Calibration){
    // nothing to do
}

void CalibrationBehaviourAbs::onButtonRepeat(const ButtonRepeat& event){
    // no-op. add event handler to behaviour in yaml to enable 
}

void CalibrationBehaviourAbs::onButtonStateChanged(const ButtonStateChanged& event){
    // no-op. add event handler to behaviour in yaml to enable 
}

void CalibrationBehaviourAbs::onEncoderStep(const EncoderStep& event){
    // no-op. add event handler to behaviour in yaml to enable 
}

void CalibrationBehaviourAbs::onHeatpadStatus(const HeatpadStatus& event){
    // no-op. add event handler to behaviour in yaml to enable 
}

void CalibrationBehaviourAbs::onTemperatureSample(const TemperatureSample& event){
    // no-op. add event handler to behaviour in yaml to enable 
}

void CalibrationBehaviourAbs::onTemperatureStatus(const TemperatureStatus& event){
    // no-op. add event handler to behaviour in yaml to enable 
}

CalibrationBehaviourAbs::FanCommandWrite CalibrationBehaviourAbs::makeFanCommandEvent(){
    return makeEvent<EventPayload::FanCommand>();
}

void CalibrationBehaviourAbs::sendEvent(const FanCommandWrite& event){
    sendEvent(event.header);
}

} // namespace Garbox