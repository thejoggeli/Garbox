// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "FanControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

FanControllerAbs::FanControllerAbs(): 
    // init members
    ControllerAbs(ComponentId::FanController){
    // nothing to do
}

FanControllerAbs::FanStatusWrite FanControllerAbs::makeFanStatusEvent(){
    return makeEvent<EventPayload::FanStatus>();
}

FanControllerAbs::FanSampleWrite FanControllerAbs::makeFanSampleEvent(){
    return makeEvent<EventPayload::FanSample>();
}

void FanControllerAbs::sendEvent(const FanStatusWrite& event){
    sendEvent(event.header);
}

void FanControllerAbs::sendEvent(const FanSampleWrite& event){
    sendEvent(event.header);
}

ControllerId FanControllerAbs::getControllerId() const {
    return ControllerId::Fan;
}

} // namespace Garbox