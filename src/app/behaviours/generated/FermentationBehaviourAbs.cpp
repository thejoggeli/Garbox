// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "FermentationBehaviourAbs.h"

namespace Garbox {

FermentationBehaviourAbs::FermentationBehaviourAbs():
    // init members
    BaseBehaviourAbs(ComponentId::FermentationBehaviour){
    // nothing to do
}

FermentationBehaviourAbs::FanCommandWrite FermentationBehaviourAbs::makeFanCommandEvent(){
    return makeEvent<EventPayload::FanCommand>();
}

FermentationBehaviourAbs::BacklightCommandWrite FermentationBehaviourAbs::makeBacklightCommandEvent(){
    return makeEvent<EventPayload::BacklightCommand>();
}

FermentationBehaviourAbs::HeatpadCommandWrite FermentationBehaviourAbs::makeHeatpadCommandEvent(){
    return makeEvent<EventPayload::HeatpadCommand>();
}

void FermentationBehaviourAbs::sendEvent(const FanCommandWrite& event){
    sendEvent(event.header);
}

void FermentationBehaviourAbs::sendEvent(const BacklightCommandWrite& event){
    sendEvent(event.header);
}

void FermentationBehaviourAbs::sendEvent(const HeatpadCommandWrite& event){
    sendEvent(event.header);
}

} // namespace Garbox