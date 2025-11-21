// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "FanControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

FanControllerAbs::FanControllerAbs(const RuntimeContext& context): 
    // init members
    ControllerAbs(ComponentId::FanController, context){
    // nothing to do
}

EventWrite<EventPayload::FanStatus> FanControllerAbs::makeFanStatusEvent(){
    return makeEvent<EventPayload::FanStatus>();
}

EventWrite<EventPayload::FanSample> FanControllerAbs::makeFanSampleEvent(){
    return makeEvent<EventPayload::FanSample>();
}

void FanControllerAbs::sendEvent(const EventWrite<EventPayload::FanStatus>& event){
    sendEvent(event.header);
}

void FanControllerAbs::sendEvent(const EventWrite<EventPayload::FanSample>& event){
    sendEvent(event.header);
}

} // namespace Garbox