// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "FanControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

FanControllerAbs::FanControllerAbs():
    // init memberes
    ControllerAbs(ComponentId::FanController, ControllerId::Fan){
    // nothing to do
}

FanStatusEvent FanControllerAbs::makeFanStatusEvent(){
    return makeEvent<EventType::FanStatus>();
}

FanSampleEvent FanControllerAbs::makeFanSampleEvent(){
    return makeEvent<EventType::FanSample>();
}

void FanControllerAbs::sendEvent(const FanStatusEvent& event){
    sendEvent(event);
}

void FanControllerAbs::sendEvent(const FanSampleEvent& event){
    sendEvent(event);
}

} // namespace Garbox