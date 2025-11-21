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

HeatpadControllerAbs::HeatpadStatusWrite HeatpadControllerAbs::makeHeatpadStatusEvent(){
    return makeEvent<EventPayload::HeatpadStatus>();
}

void HeatpadControllerAbs::sendEvent(const HeatpadStatusWrite& event){
    sendEvent(event.header);
}

} // namespace Garbox