// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "FermentationBehaviourAbs.h"

namespace Garbox {

FermentationBehaviourAbs::FermentationBehaviourAbs(const RuntimeContext& context):
    // init members
    BaseBehaviourAbs(ComponentId::FermentationBehaviour, context){
    // nothing to do
}

EventWrite<EventPayload::FanCommand> FermentationBehaviourAbs::makeFanCommandEvent(){
    return makeEvent<EventPayload::FanCommand>();
}

EventWrite<EventPayload::BacklightCommand> FermentationBehaviourAbs::makeBacklightCommandEvent(){
    return makeEvent<EventPayload::BacklightCommand>();
}

EventWrite<EventPayload::HeatpadCommand> FermentationBehaviourAbs::makeHeatpadCommandEvent(){
    return makeEvent<EventPayload::HeatpadCommand>();
}

void FermentationBehaviourAbs::sendEvent(const EventWrite<EventPayload::FanCommand>& event){
    sendEvent(event.header);
}

void FermentationBehaviourAbs::sendEvent(const EventWrite<EventPayload::BacklightCommand>& event){
    sendEvent(event.header);
}

void FermentationBehaviourAbs::sendEvent(const EventWrite<EventPayload::HeatpadCommand>& event){
    sendEvent(event.header);
}

} // namespace Garbox