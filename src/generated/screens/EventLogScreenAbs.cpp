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
    mRoot(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()){}

void EventLogScreenAbs::init(ComponentHostIfc& host){

    // init lvgl container
    mRoot.setRawSize(mScreenWidth, mScreenHeight);
    mRoot.setBorder(0, lv_color_hex(0x000000));
    mRoot.setRadius(0);
    mRoot.setPad(0, 0, 0, 0);
    mRoot.setBgOpa(LV_OPA_COVER);
    mRoot.setScrollable(false);

    // calls onInit()
    ScreenAbs::init(host);
}

void EventLogScreenAbs::updateScreen(){
    ScreenAbs::updateScreen();
}

void EventLogScreenAbs::becomeEnabled(){
    mRoot.setScreen();
    ScreenAbs::becomeEnabled();
}

void EventLogScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

void EventLogScreenAbs::setBackgroundColor(uint32_t color){
    mRoot.setBgColor(lv_color_hex(color));
}

} // namespace Garbox