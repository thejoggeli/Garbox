// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FermentationBehaviourAbs.h"

namespace Garbox {

FermentationBehaviourAbs::FermentationBehaviourAbs() : BehaviourAbs(ComponentId::FermentationBehaviour, BehaviourId::Fermentation){
    // nothing to do
}

FanCommandEvent FermentationBehaviourAbs::makeFanCommandEvent(){
    return ComponentAbs::makeEvent<EventType::FanCommand>();
}

DisplayCommandEvent FermentationBehaviourAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

HeatpadCommandEvent FermentationBehaviourAbs::makeHeatpadCommandEvent(){
    return ComponentAbs::makeEvent<EventType::HeatpadCommand>();
}

void FermentationBehaviourAbs::sendEvent(const FanCommandEvent& event){
    sendEventToHost(event.header());
}

void FermentationBehaviourAbs::sendEvent(const DisplayCommandEvent& event){
    sendEventToHost(event.header());
}

void FermentationBehaviourAbs::sendEvent(const HeatpadCommandEvent& event){
    sendEventToHost(event.header());
}

FermentationBehaviourStates& FermentationBehaviourAbs::states(){
    return mStates.value();
}

} // namespace Garbox