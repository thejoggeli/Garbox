#include "ScreenAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ScreenAbs::ScreenAbs(ComponentId componentId, ScreenId ScreenId): 
    // init memberes
    ComponentAbs(ComponentType::Screen, componentId),
    mScreenId(ScreenId){}

void ScreenAbs::init(ComponentHostIfc& host){
    ComponentAbs::init(host);
}

void ScreenAbs::becomeEnabled(){
    mFirstUpdate = true;
    ComponentAbs::becomeEnabled();
}

void ScreenAbs::becomeDisabled(){
    mFirstUpdate = true;
    ComponentAbs::becomeDisabled();
}

void ScreenAbs::render(){
    onRender();
    mFirstUpdate = false;
}

} // namespace
