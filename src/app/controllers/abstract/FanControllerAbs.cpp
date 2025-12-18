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

FanStatusState& FanControllerAbs::writeFanStatusState(){
    return *mFanStatusState;
}

FanSampleState& FanControllerAbs::writeFanSampleState(){
    return *mFanSampleState;
}

void FanControllerAbs::injectFanStatusState(FanStatusState* state){
    mFanStatusState = state;
}

void FanControllerAbs::injectFanSampleState(FanSampleState* state){
    mFanSampleState = state;
}

} // namespace Garbox