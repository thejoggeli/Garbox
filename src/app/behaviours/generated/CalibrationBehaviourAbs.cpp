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

CalibrationBehaviourAbs::FanCommandWrite CalibrationBehaviourAbs::makeFanCommandEvent(){
    return makeEvent<EventPayload::FanCommand>();
}

void CalibrationBehaviourAbs::sendEvent(const FanCommandWrite& event){
    sendEvent(event.header);
}

} // namespace Garbox