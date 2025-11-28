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

BacklightCommandEvent DebugScreenAbs::makeBacklightCommandEvent(){
    return ScreenAbs::makeEvent<EventType::BacklightCommand>();
}

void DebugScreenAbs::sendEvent(const BacklightCommandEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox