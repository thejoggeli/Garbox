// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

HeatpadControllerAbs::HeatpadControllerAbs() : ControllerAbs(ComponentId::HeatpadController, ControllerId::Heatpad){
    // nothing to do
}

HeatpadStatusEvent HeatpadControllerAbs::makeHeatpadStatusEvent(){
    return ComponentAbs::makeEvent<EventType::HeatpadStatus>();
}

HeatpadSampleEvent HeatpadControllerAbs::makeHeatpadSampleEvent(){
    return ComponentAbs::makeEvent<EventType::HeatpadSample>();
}

void HeatpadControllerAbs::sendEvent(const HeatpadStatusEvent& event){
    sendEventToHost(event.header());
}

void HeatpadControllerAbs::sendEvent(const HeatpadSampleEvent& event){
    sendEventToHost(event.header());
}

HeatpadControllerAbs::States& HeatpadControllerAbs::states(){
    return mStates.value();
}

void HeatpadControllerAbs::bindStates(
    HeatpadStatusState& heatpadStatus,
    HeatpadSampleState& heatpadSample
){
    mStates.emplace(
        heatpadStatus,
        heatpadSample
    );
}

} // namespace Garbox