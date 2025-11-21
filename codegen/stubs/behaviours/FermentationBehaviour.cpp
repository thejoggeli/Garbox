// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#include "FermentationBehaviour.h"

namespace Garbox {

FermentationBehaviour::FermentationBehaviour(const RuntimeContext& context):
    // init members
    FermentationBehaviourAbs(context){
    // nothing to do
}

void FermentationBehaviour::onInit(){
    // nothing to do
}

void FermentationBehaviour::onStart(){
    // nothing to do
}

void FermentationBehaviour::onBecomeActive(){
    // nothing to do
}

void FermentationBehaviour::onBecomeInactive(){
    // nothing to do
}

void FermentationBehaviour::onLogicTick(){
    // nothing to do
}

void FermentationBehaviour::onHeartbeat(const EventRead<EventPayload::Heartbeat>& event){
    // nothing to do
}

void FermentationBehaviour::onFanStatus(const EventRead<EventPayload::FanStatus>& event){
    // nothing to do
}

void FermentationBehaviour::onFanSample(const EventRead<EventPayload::FanSample>& event){
    // nothing to do
}

void FermentationBehaviour::onHeatpadStatus(const EventRead<EventPayload::HeatpadStatus>& event){
    // nothing to do
}

void FermentationBehaviour::onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event){
    // nothing to do
}

void FermentationBehaviour::onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event){
    // nothing to do
}

void FermentationBehaviour::onButton(const EventRead<EventPayload::Button>& event){
    // nothing to do
}

void FermentationBehaviour::onButtonRepeat(const EventRead<EventPayload::ButtonRepeat>& event){
    // nothing to do
}

void FermentationBehaviour::onEncoderStep(const EventRead<EventPayload::EncoderStep>& event){
    // nothing to do
}

} // namespace Garbox