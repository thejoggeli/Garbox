// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "CalibrationBehaviourAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

CalibrationBehaviourAbs::CalibrationBehaviourAbs() : BehaviourAbs(ComponentId::CalibrationBehaviour, BehaviourId::Calibration){
    // nothing to do
}

FanCommandEvent CalibrationBehaviourAbs::makeFanCommandEvent(){
    return ComponentAbs::makeEvent<EventType::FanCommand>();
}

void CalibrationBehaviourAbs::sendEvent(const FanCommandEvent& event){
    sendEventToHost(event.header());
}
 
} // namespace Garbox