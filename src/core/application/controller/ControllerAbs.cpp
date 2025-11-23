#include "ControllerAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ControllerAbs::ControllerAbs(ComponentId componentId, ControllerId controllerId): 
    // init memberes
    ComponentAbs(ComponentType::Controller, componentId),
    mControllerId(controllerId){
    // nothing to do
}

void ControllerAbs::setControllerHost(ControllerHostIfc& host){
    setComponentHost(host);
}

} // namespace