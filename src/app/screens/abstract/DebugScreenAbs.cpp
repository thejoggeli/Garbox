// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs() : ScreenAbs(ComponentId::DebugScreen, ScreenId::Debug){
    // nothing to do
}

DisplayCommandEvent DebugScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void DebugScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox