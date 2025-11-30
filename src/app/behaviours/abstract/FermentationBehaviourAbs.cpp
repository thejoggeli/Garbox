// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FermentationBehaviourAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

FermentationBehaviourAbs::FermentationBehaviourAbs() : BehaviourAbs(ComponentId::FermentationBehaviour, BehaviourId::Fermentation){
    // nothing to do
}

void FermentationBehaviourAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Logic: onLogicTick(); break;
        default: TriggerDebug("FermentationBehaviourAbs", "received unhandled tick");
    };
};

void FermentationBehaviourAbs::receiveEvent(const EventHeader* header){
    switch(header->type){
        case EventType::Heartbeat: onHeartbeat(HeartbeatEvent(header)); break;
        case EventType::FanStatus: onFanStatus(FanStatusEvent(header)); break;
        case EventType::FanSample: onFanSample(FanSampleEvent(header)); break;
        case EventType::HeatpadStatus: onHeatpadStatus(HeatpadStatusEvent(header)); break;
        case EventType::TemperatureStatus: onTemperatureStatus(TemperatureStatusEvent(header)); break;
        case EventType::TemperatureSample: onTemperatureSample(TemperatureSampleEvent(header)); break;
        case EventType::ButtonStateChanged: onButtonStateChanged(ButtonStateChangedEvent(header)); break;
        case EventType::ButtonRepeat: onButtonRepeat(ButtonRepeatEvent(header)); break;
        case EventType::EncoderStep: onEncoderStep(EncoderStepEvent(header)); break;
        default: TriggerDebug("FermentationBehaviourAbs", "received unhandled event");
    };
};

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