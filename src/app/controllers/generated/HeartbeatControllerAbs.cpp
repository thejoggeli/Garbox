// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "HeartbeatControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

HeartbeatControllerAbs::HeartbeatControllerAbs(const RuntimeContext& context): 
    // init members
    ControllerAbs(ComponentId::HeartbeatController, context){
    // nothing to do
}

EventWrite<EventPayload::Heartbeat> HeartbeatControllerAbs::makeHeartbeatEvent(){
    return makeEvent<EventPayload::Heartbeat>();
}

void HeartbeatControllerAbs::sendEvent(const EventWrite<EventPayload::Heartbeat>& event){
    sendEvent(event.header);
}

} // namespace Garbox