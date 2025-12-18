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

FanStatusState& FanControllerAbs::writeState(FanStatusType type){
    return *mFanStatusState;
}

FanSampleState& FanControllerAbs::writeState(FanSampleType type){
    return *mFanSampleState;
}

void FanControllerAbs::injectWritableState(FanStatusState* state){
    mFanStatusState = state;
}

void FanControllerAbs::injectWritableState(FanSampleState* state){
    mFanSampleState = state;
}

} // namespace Garbox