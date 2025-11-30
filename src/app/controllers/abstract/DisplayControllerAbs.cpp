// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DisplayControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DisplayControllerAbs::DisplayControllerAbs() : ControllerAbs(ComponentId::DisplayController, ControllerId::Display){
    // nothing to do
}

DisplayStatusEvent DisplayControllerAbs::makeDisplayStatusEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayStatus>();
}

void DisplayControllerAbs::sendEvent(const DisplayStatusEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox