// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FermentationBehaviourAbs.h"

namespace Garbox {

FermentationBehaviourAbs::FermentationBehaviourAbs(): 
    // init memberes
    BehaviourAbs(ComponentId::FermentationBehaviour, BehaviourId::Fermentation){
    // nothing to do
}

FanCommandEvent FermentationBehaviourAbs::makeFanCommandEvent(){
    return BehaviourAbs::makeEvent<EventType::FanCommand>();
}

DisplayCommandEvent FermentationBehaviourAbs::makeDisplayCommandEvent(){
    return BehaviourAbs::makeEvent<EventType::DisplayCommand>();
}

HeatpadCommandEvent FermentationBehaviourAbs::makeHeatpadCommandEvent(){
    return BehaviourAbs::makeEvent<EventType::HeatpadCommand>();
}

FermentationStatusEvent FermentationBehaviourAbs::makeFermentationStatusEvent(){
    return BehaviourAbs::makeEvent<EventType::FermentationStatus>();
}

void FermentationBehaviourAbs::sendEvent(const FanCommandEvent& event){
    publishEvent(event.header());
}

void FermentationBehaviourAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

void FermentationBehaviourAbs::sendEvent(const HeatpadCommandEvent& event){
    publishEvent(event.header());
}

void FermentationBehaviourAbs::sendEvent(const FermentationStatusEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox