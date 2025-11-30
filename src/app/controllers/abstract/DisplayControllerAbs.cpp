// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DisplayControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DisplayControllerAbs::DisplayControllerAbs() : ControllerAbs(ComponentId::DisplayController, ControllerId::Display){
    // nothing to do
}

void DisplayControllerAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Render: onRenderTick(); break;
        default: TriggerDebug("DisplayControllerAbs", "received unhandled tick");
    };
};

void DisplayControllerAbs::receiveEvent(const EventHeader* header){
    switch(header->type){
        case EventType::DisplayCommand: onDisplayCommand(DisplayCommandEvent(header)); break;
        default: TriggerDebug("DisplayControllerAbs", "received unhandled event");
    };
};

DisplayStatusEvent DisplayControllerAbs::makeDisplayStatusEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayStatus>();
}

void DisplayControllerAbs::sendEvent(const DisplayStatusEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox