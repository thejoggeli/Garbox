#include "ScreenAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ScreenAbs::ScreenAbs(ComponentId componentId, ScreenId ScreenId): 
    // init memberes
    ComponentAbs(ComponentType::Screen, componentId),
    mScreenId(ScreenId){
    // nothing to do
}

void ScreenAbs::init(ComponentHostIfc& host){
    ComponentAbs::init(host);
    initScreen();
};

void ScreenAbs::initScreen(){
    // nothing to do
}

void ScreenAbs::updateScreen(){
    onUpdateScreen();
}

} // namespace
