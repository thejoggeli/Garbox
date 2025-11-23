// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "HeartbeatControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

HeartbeatControllerAbs::HeartbeatControllerAbs():
    // init memberes
    ControllerAbs(ComponentId::HeartbeatController, ControllerId::Heartbeat){
    // nothing to do
}

HeartbeatEvent HeartbeatControllerAbs::makeHeartbeatEvent(){
    return makeEvent<EventType::Heartbeat>();
}

void HeartbeatControllerAbs::sendEvent(const HeartbeatEvent& event){
    sendEvent(event);
}

} // namespace Garbox