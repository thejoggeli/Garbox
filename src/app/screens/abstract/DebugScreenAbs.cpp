// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"
#include <math.h>
#include "shared/types/ComponentId.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs() : ScreenAbs(ComponentId::DebugScreen, ScreenId::Debug, 0){}

void DebugScreenAbs::init(ComponentHostIfc& host){
    ScreenAbs::init(host);
    // no model fields configured
}

DisplayCommandEvent DebugScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void DebugScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox