// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeartbeatControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

HeartbeatControllerAbs::HeartbeatControllerAbs() : ControllerAbs(ComponentId::HeartbeatController, ControllerId::Heartbeat){
    // nothing to do
}

HeartbeatEvent HeartbeatControllerAbs::makeHeartbeatEvent(){
    return ComponentAbs::makeEvent<EventType::Heartbeat>();
}

void HeartbeatControllerAbs::sendEvent(const HeartbeatEvent& event){
    sendEventToHost(event.header());
}
 
} // namespace Garbox