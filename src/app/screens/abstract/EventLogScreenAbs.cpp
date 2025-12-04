// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "EventLogScreenAbs.h"
#include <math.h>
#include "core/lvgl/LvglProvider.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

EventLogScreenAbs::EventLogScreenAbs(): 
    ScreenAbs(ComponentId::EventLogScreen, ScreenId::EventLog),
    mContainer(LvglProvider::Root()),
    mDisplayWidth(LvglProvider::GetDisplayWidth()),
    mDisplayHeight(LvglProvider::GetDisplayHeight()){}

void EventLogScreenAbs::init(ComponentHostIfc& host){
    ScreenAbs::init(host);

    // init lvgl container
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

void EventLogScreenAbs::updateScreen(){
    ScreenAbs::updateScreen();
}

void EventLogScreenAbs::becomeEnabled(){
    mContainer.setHidden(false);
    ScreenAbs::becomeEnabled();
}

void EventLogScreenAbs::becomeDisabled(){
    mContainer.setHidden(true);
    ScreenAbs::becomeDisabled();
}

void EventLogScreenAbs::setBackgroundColor(uint32_t color){
    mContainer.setBgColor(lv_color_hex(color));
}

} // namespace Garbox