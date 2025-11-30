// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs() : ScreenAbs(ComponentId::MainScreen, ScreenId::Main){
    // nothing to do
}

void MainScreenAbs::receiveTick(TickPhase phase){
    // no tick phases configured
};

void MainScreenAbs::receiveEvent(const EventHeader* header){
    switch(header->type){
        case EventType::FanStatus: onFanStatus(FanStatusEvent(header)); break;
        case EventType::FanSample: onFanSample(FanSampleEvent(header)); break;
        case EventType::HeatpadStatus: onHeatpadStatus(HeatpadStatusEvent(header)); break;
        case EventType::HeatpadSample: onHeatpadSample(HeatpadSampleEvent(header)); break;
        case EventType::TemperatureStatus: onTemperatureStatus(TemperatureStatusEvent(header)); break;
        case EventType::TemperatureSample: onTemperatureSample(TemperatureSampleEvent(header)); break;
        case EventType::ActiveBehaviourChanged: onActiveBehaviourChanged(ActiveBehaviourChangedEvent(header)); break;
        case EventType::FermentationStatus: onFermentationStatus(FermentationStatusEvent(header)); break;
        case EventType::DisplayStatus: onDisplayStatus(DisplayStatusEvent(header)); break;
        default: TriggerDebug("MainScreenAbs", "received unhandled event");
    };
};

DisplayCommandEvent MainScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void MainScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox