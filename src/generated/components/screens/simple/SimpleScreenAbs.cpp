// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "SimpleScreenAbs.h"
#include "core/lvgl/LvglProvider.h"

namespace Garbox {

SimpleScreenAbs::SimpleScreenAbs(): 
    ScreenAbs(ComponentId::SimpleScreen, ScreenId::Simple),
    mGui(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()){}

void SimpleScreenAbs::init(ComponentHostIfc& host){

    // init gui
    mGui.init();

    // calls onInit()
    ScreenAbs::init(host);
}

void SimpleScreenAbs::render(){
    ScreenAbs::render();
}

void SimpleScreenAbs::becomeEnabled(){
    mGui.show();

    ScreenAbs::becomeEnabled();
}

void SimpleScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

SimpleScreenGui::Objects& SimpleScreenAbs::gui(){
    return mGui.objects();
}

void SimpleScreenAbs::setBackgroundColor(uint32_t color){
    mGui.objects().root.setBgColor(lv_color_hex(color));
}

} // namespace Garbox