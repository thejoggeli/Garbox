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

HeatpadStatusState& HeatpadControllerAbs::stateHeatpadStatus(){
    return *mHeatpadStatusState;
}

HeatpadSampleState& HeatpadControllerAbs::stateHeatpadSample(){
    return *mHeatpadSampleState;
}

void HeatpadControllerAbs::injectHeatpadStatusState(HeatpadStatusState* state){
    mHeatpadStatusState = state;
}

void HeatpadControllerAbs::injectHeatpadSampleState(HeatpadSampleState* state){
    mHeatpadSampleState = state;
}

} // namespace Garbox