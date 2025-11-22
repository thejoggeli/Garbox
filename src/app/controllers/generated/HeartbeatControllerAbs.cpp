// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "HeartbeatControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

HeartbeatControllerAbs::HeartbeatControllerAbs(): 
    // init members
    ControllerAbs(ComponentId::HeartbeatController){
    // nothing to do
}

HeartbeatControllerAbs::HeartbeatWrite HeartbeatControllerAbs::makeHeartbeatEvent(){
    return makeEvent<EventPayload::Heartbeat>();
}

void HeartbeatControllerAbs::sendEvent(const HeartbeatWrite& event){
    sendEvent(event.header);
}

} // namespace Garbox