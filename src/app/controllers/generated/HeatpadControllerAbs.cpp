// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

HeatpadControllerAbs::HeatpadControllerAbs():
    // init memberes
    ControllerAbs(ComponentId::HeatpadController, ControllerId::Heatpad){
    // nothing to do
}

HeatpadStatusEvent HeatpadControllerAbs::makeHeatpadStatusEvent(){
    return ControllerAbs::makeEvent<EventType::HeatpadStatus>();
}

HeatpadSampleEvent HeatpadControllerAbs::makeHeatpadSampleEvent(){
    return ControllerAbs::makeEvent<EventType::HeatpadSample>();
}

void HeatpadControllerAbs::sendEvent(const HeatpadStatusEvent& event){
    publishEvent(event.header());
}

void HeatpadControllerAbs::sendEvent(const HeatpadSampleEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox