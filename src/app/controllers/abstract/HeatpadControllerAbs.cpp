// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

HeatpadControllerAbs::HeatpadControllerAbs() : ControllerAbs(ComponentId::HeatpadController, ControllerId::Heatpad){
    // nothing to do
}

void HeatpadControllerAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Input: onInputTick(); break;
        case TickPhase::Output: onOutputTick(); break;
        default: TriggerDebug("HeatpadControllerAbs", "received unhandled tick");
    };
};

void HeatpadControllerAbs::receiveEvent(const EventHeader* header){
    switch(header->type){
        case EventType::HeatpadCommand: onHeatpadCommand(HeatpadCommandEvent(header)); break;
        default: TriggerDebug("HeatpadControllerAbs", "received unhandled event");
    };
};

HeatpadStatusEvent HeatpadControllerAbs::makeHeatpadStatusEvent(){
    return ComponentAbs::makeEvent<EventType::HeatpadStatus>();
}

HeatpadSampleEvent HeatpadControllerAbs::makeHeatpadSampleEvent(){
    return ComponentAbs::makeEvent<EventType::HeatpadSample>();
}

void HeatpadControllerAbs::sendEvent(const HeatpadStatusEvent& event){
    publishEvent(event.header());
}

void HeatpadControllerAbs::sendEvent(const HeatpadSampleEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox