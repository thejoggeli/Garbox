// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "StateLogScreenAbs.h"
#include "core/lvgl/LvglProvider.h"

namespace Garbox {

StateLogScreenAbs::StateLogScreenAbs(): 
    ScreenAbs(ComponentId::StateLogScreen, ScreenId::StateLog),
    mRoot(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()){}

void StateLogScreenAbs::init(ComponentHostIfc& host){

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

void StateLogScreenAbs::render(){
    ScreenAbs::render();
}

void StateLogScreenAbs::becomeEnabled(){
    mRoot.setScreen();

    ScreenAbs::becomeEnabled();
}

void StateLogScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

void StateLogScreenAbs::setBackgroundColor(uint32_t color){
    mRoot.setBgColor(lv_color_hex(color));
}

} // namespace Garbox