// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeartbeatControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

HeartbeatControllerAbs::HeartbeatControllerAbs() : ControllerAbs(ComponentId::HeartbeatController, ControllerId::Heartbeat){
    // nothing to do
}

void HeartbeatControllerAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Heartbeat: onHeartbeatTick(); break;
        default: TriggerDebug("HeartbeatControllerAbs", "received unhandled tick");
    };
};

void HeartbeatControllerAbs::receiveEvent(const EventHeader* header){
    // no events configured
};

HeartbeatEvent HeartbeatControllerAbs::makeHeartbeatEvent(){
    return ComponentAbs::makeEvent<EventType::Heartbeat>();
}

void HeartbeatControllerAbs::sendEvent(const HeartbeatEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox