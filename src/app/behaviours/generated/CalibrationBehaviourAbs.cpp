// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "CalibrationBehaviourAbs.h"

namespace Garbox {

CalibrationBehaviourAbs::CalibrationBehaviourAbs(): 
    // init memberes
    BehaviourAbs(ComponentId::CalibrationBehaviour, BehaviourId::Calibration){
    // nothing to do
}

FanCommandEvent CalibrationBehaviourAbs::makeFanCommandEvent(){
    return BehaviourAbs::makeEvent<EventType::FanCommand>();
}

void CalibrationBehaviourAbs::sendEvent(const FanCommandEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox