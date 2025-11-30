// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs() : ScreenAbs(ComponentId::DebugScreen, ScreenId::Debug){
    // nothing to do
}

void DebugScreenAbs::receiveTick(TickPhase phase){
    // no tick phases configured
};

void DebugScreenAbs::receiveEvent(const EventHeader* header){
    switch(header->type){
        case EventType::Heartbeat: onHeartbeat(HeartbeatEvent(header)); break;
        default: TriggerDebug("DebugScreenAbs", "received unhandled event");
    };
};

DisplayCommandEvent DebugScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void DebugScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox