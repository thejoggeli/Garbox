// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "CalibrationBehaviourAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

CalibrationBehaviourAbs::CalibrationBehaviourAbs() : BehaviourAbs(ComponentId::CalibrationBehaviour, BehaviourId::Calibration){
    // nothing to do
}

void CalibrationBehaviourAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Logic: onLogicTick(); break;
        default: TriggerDebug("CalibrationBehaviourAbs", "received unhandled tick");
    };
};

void CalibrationBehaviourAbs::receiveEvent(const EventHeader* header){
    switch(header->type){
        case EventType::Heartbeat: onHeartbeat(HeartbeatEvent(header)); break;
        case EventType::FanStatus: onFanStatus(FanStatusEvent(header)); break;
        case EventType::FanSample: onFanSample(FanSampleEvent(header)); break;
        default: TriggerDebug("CalibrationBehaviourAbs", "received unhandled event");
    };
};

FanCommandEvent CalibrationBehaviourAbs::makeFanCommandEvent(){
    return ComponentAbs::makeEvent<EventType::FanCommand>();
}

void CalibrationBehaviourAbs::sendEvent(const FanCommandEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox