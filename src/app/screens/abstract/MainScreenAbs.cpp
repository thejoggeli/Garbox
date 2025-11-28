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

DisplayCommandEvent MainScreenAbs::makeDisplayCommandEvent(){
    return ScreenAbs::makeEvent<EventType::DisplayCommand>();
}

void MainScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox