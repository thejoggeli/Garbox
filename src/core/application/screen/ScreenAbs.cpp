#include "ScreenAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ScreenAbs::ScreenAbs(ComponentId componentId, ScreenId ScreenId, uint32_t dispatcherCapacity, LvglContext& lvgl): 
    // init memberes
    ComponentAbs(ComponentType::Screen, componentId),
    mScreenId(ScreenId),
    mLvgl(lvgl),
    mContainer(lvgl.root()),
    mDirtyDispatcher(dispatcherCapacity){

    // initialize display size
    mLvgl.getDisplaySize(mDisplayWidth, mDisplayHeight);
    mContainer.setHidden(true);
    mContainer.setSize(mDisplayWidth, mDisplayHeight);
    mContainer.setBorder(0, lv_color_hex(0x000000));
    mContainer.setRadius(0);
    mContainer.setPad(0, 0, 0, 0);
    mContainer.setBgOpacity(LV_OPA_COVER);
    mContainer.setScrollable(false);
    mContainer.clearFlag(LV_OBJ_FLAG_SCROLL_CHAIN);
    mContainer.clearFlag(LV_OBJ_FLAG_SCROLL_ON_FOCUS);
}

void ScreenAbs::init(ComponentHostIfc& host){
    ComponentAbs::init(host);
}

void ScreenAbs::becomeEnabled(){
    mContainer.setHidden(false);
    mFirstUpdate = true;
    ComponentAbs::becomeEnabled();
}

void ScreenAbs::becomeDisabled(){
    mContainer.setHidden(true);
    mFirstUpdate = true;
    ComponentAbs::becomeDisabled();
}

void ScreenAbs::updateScreen(){
    onUpdateScreen();
    mDispatchedCount += mDirtyDispatcher.getDirtyCount();
    mDirtyDispatcher.dispatch();
    mFirstUpdate = true;
}

void ScreenAbs::setBackgroundColor(uint32_t color){
    mContainer.setBgColor(lv_color_hex(color));
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
