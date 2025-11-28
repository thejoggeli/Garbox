// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs(): 
    // init memberes
    ScreenAbs(ComponentId::DebugScreen, ScreenId::Debug){
    // nothing to do
}

DisplayCommandEvent DebugScreenAbs::makeDisplayCommandEvent(){
    return ScreenAbs::makeEvent<EventType::DisplayCommand>();
}

void DebugScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox