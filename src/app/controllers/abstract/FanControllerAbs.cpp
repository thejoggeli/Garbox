// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FanControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

FanControllerAbs::FanControllerAbs() : ControllerAbs(ComponentId::FanController, ControllerId::Fan){
    // nothing to do
}

void FanControllerAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Input: onInputTick(); break;
        case TickPhase::Output: onOutputTick(); break;
        default: TriggerDebug("FanControllerAbs", "received unhandled tick");
    };
};

void FanControllerAbs::receiveEvent(const EventHeader* header){
    switch(header->type){
        case EventType::FanCommand: onFanCommand(FanCommandEvent(header)); break;
        default: TriggerDebug("FanControllerAbs", "received unhandled event");
    };
};

FanStatusEvent FanControllerAbs::makeFanStatusEvent(){
    return ComponentAbs::makeEvent<EventType::FanStatus>();
}

FanSampleEvent FanControllerAbs::makeFanSampleEvent(){
    return ComponentAbs::makeEvent<EventType::FanSample>();
}

void FanControllerAbs::sendEvent(const FanStatusEvent& event){
    publishEvent(event.header());
}

void FanControllerAbs::sendEvent(const FanSampleEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox