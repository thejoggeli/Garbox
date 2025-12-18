// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FanControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

FanControllerAbs::FanControllerAbs() : ControllerAbs(ComponentId::FanController, ControllerId::Fan){
    // nothing to do
}

FanStatusEvent FanControllerAbs::makeFanStatusEvent(){
    return ComponentAbs::makeEvent<EventType::FanStatus>();
}

FanSampleEvent FanControllerAbs::makeFanSampleEvent(){
    return ComponentAbs::makeEvent<EventType::FanSample>();
}

void FanControllerAbs::sendEvent(const FanStatusEvent& event){
    sendEventToHost(event.header());
}

void FanControllerAbs::sendEvent(const FanSampleEvent& event){
    sendEventToHost(event.header());
}

FanControllerAbs::States& FanControllerAbs::states(){
    return mStates.value();
}

void FanControllerAbs::bindStates(
    FanStatusState& fanStatus,
    FanSampleState& fanSample
){
    mStates.emplace(
        fanStatus,
        fanSample
    );
}

} // namespace Garbox