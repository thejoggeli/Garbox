// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"
#include <math.h>
#include "core/lvgl/LvglProvider.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs(): 
    ScreenAbs(ComponentId::DebugScreen, ScreenId::Debug),
    mRoot(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()){}

void DebugScreenAbs::init(ComponentHostIfc& host){

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

void DebugScreenAbs::updateScreen(){
    ScreenAbs::updateScreen();
}

void DebugScreenAbs::becomeEnabled(){
    mRoot.setScreen();
    ScreenAbs::becomeEnabled();
}

void DebugScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

DisplayCommandEvent DebugScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void DebugScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

void DebugScreenAbs::setBackgroundColor(uint32_t color){
    mRoot.setBgColor(lv_color_hex(color));
}

} // namespace Garbox