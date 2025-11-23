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
    return makeEvent<EventType::HeatpadStatus>();
}

HeatpadSampleEvent HeatpadControllerAbs::makeHeatpadSampleEvent(){
    return makeEvent<EventType::HeatpadSample>();
}

void HeatpadControllerAbs::sendEvent(const HeatpadStatusEvent& event){
    sendEvent(event);
}

void HeatpadControllerAbs::sendEvent(const HeatpadSampleEvent& event){
    sendEvent(event);
}

} // namespace Garbox