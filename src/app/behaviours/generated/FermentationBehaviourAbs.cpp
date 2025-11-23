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
    return makeEvent<EventType::FanCommand>();
}

BacklightCommandEvent FermentationBehaviourAbs::makeBacklightCommandEvent(){
    return makeEvent<EventType::BacklightCommand>();
}

HeatpadCommandEvent FermentationBehaviourAbs::makeHeatpadCommandEvent(){
    return makeEvent<EventType::HeatpadCommand>();
}

void FermentationBehaviourAbs::sendEvent(const FanCommandEvent& event){
    sendEvent(event);
}

void FermentationBehaviourAbs::sendEvent(const BacklightCommandEvent& event){
    sendEvent(event);
}

void FermentationBehaviourAbs::sendEvent(const HeatpadCommandEvent& event){
    sendEvent(event);
}

} // namespace Garbox