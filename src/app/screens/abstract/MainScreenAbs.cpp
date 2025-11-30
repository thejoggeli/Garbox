// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs() : ScreenAbs(ComponentId::MainScreen, ScreenId::Main){
    // nothing to do
}

DisplayCommandEvent MainScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void MainScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox