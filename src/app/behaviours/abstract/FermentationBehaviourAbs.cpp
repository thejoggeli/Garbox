// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FermentationBehaviourAbs.h"
#include "shared/types/ComponentId.h"

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

FermentationStatusEvent FermentationBehaviourAbs::makeFermentationStatusEvent(){
    return ComponentAbs::makeEvent<EventType::FermentationStatus>();
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

void FermentationBehaviourAbs::sendEvent(const FermentationStatusEvent& event){
    sendEventToHost(event.header());
}

FermentationBehaviourAbs::States& FermentationBehaviourAbs::states(){
    return mStates.value();
}

void FermentationBehaviourAbs::bindStates(
    FermentationStatusState& fermentationStatus,
    const FanSampleState& fanSample,
    const FanStatusState& fanStatus,
    const TemperatureSampleState& temperatureSample,
    const TemperatureStatusState& temperatureStatus
){
    mStates.emplace(
        fermentationStatus,
        fanSample,
        fanStatus,
        temperatureSample,
        temperatureStatus
    );
}

} // namespace Garbox