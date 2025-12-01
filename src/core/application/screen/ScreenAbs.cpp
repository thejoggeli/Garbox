#include "ScreenAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ScreenAbs::ScreenAbs(ComponentId componentId, ScreenId ScreenId, uint32_t dispatcherCapacity): 
    // init memberes
    ComponentAbs(ComponentType::Screen, componentId),
    mScreenId(ScreenId),
    mDirtyDispatcher(dispatcherCapacity){
    // nothing to do
}

void ScreenAbs::init(ComponentHostIfc& host){
    ComponentAbs::init(host);
    onInitScreen();
}

void ScreenAbs::updateScreen(){
    onUpdateScreen();
    mDispatchedCount += mDirtyDispatcher.getDirtyCount();
    mDirtyDispatcher.dispatch();
}

void ScreenAbs::registerUpdateHandler(DirtyDispatcher::HandlerFunction handler, void* context){
    mDirtyDispatcher.registerHandler(handler, context);
}

void ScreenAbs::markDirtyRaw(uint32_t index){
    mDirtyDispatcher.markDirty(index);
}

bool ScreenAbs::isMarkedDirtyRaw(uint32_t index) const {
    return mDirtyDispatcher.isMarkedDirty(index);
}

} // namespace
