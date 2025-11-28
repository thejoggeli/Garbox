// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs(): 
    // init memberes
    ScreenAbs(ComponentId::MainScreen, ScreenId::Main){
    // nothing to do
}

BacklightCommandEvent MainScreenAbs::makeBacklightCommandEvent(){
    return ScreenAbs::makeEvent<EventType::BacklightCommand>();
}

void MainScreenAbs::sendEvent(const BacklightCommandEvent& event){
    publishEvent(event.header());
}

void updateFanSample(const FanSamplePayload& payload){
    // nothing to do
}

void updateFanStatus(const FanStatusPayload& payload){
    // nothing to do
}

void updateHeatpadSample(const HeatpadSamplePayload& payload){
    // nothing to do
}

void updateHeatpadStatus(const HeatpadStatusPayload& payload){
    // nothing to do
}

} // namespace Garbox