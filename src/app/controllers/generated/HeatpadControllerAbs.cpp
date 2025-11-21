// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "HeatpadControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

HeatpadControllerAbs::HeatpadControllerAbs(const RuntimeContext& context): 
    // init members
    ControllerAbs(ComponentId::HeatpadController, context){
    // nothing to do
}

EventWrite<EventPayload::HeatpadStatus> HeatpadControllerAbs::makeHeatpadStatusEvent(){
    return makeEvent<EventPayload::HeatpadStatus>();
}

void HeatpadControllerAbs::sendEvent(const EventWrite<EventPayload::HeatpadStatus>& event){
    sendEvent(event.header);
}

} // namespace Garbox