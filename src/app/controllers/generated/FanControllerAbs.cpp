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
    return ControllerAbs::makeEvent<EventType::FanStatus>();
}

FanSampleEvent FanControllerAbs::makeFanSampleEvent(){
    return ControllerAbs::makeEvent<EventType::FanSample>();
}

void FanControllerAbs::sendEvent(const FanStatusEvent& event){
    publishEvent(event.header());
}

void FanControllerAbs::sendEvent(const FanSampleEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox