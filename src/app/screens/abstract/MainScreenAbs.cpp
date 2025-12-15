// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "MainScreenAbs.h"
#include <math.h>
#include "core/lvgl/LvglProvider.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

MainScreenAbs::MainScreenAbs(): 
    ScreenAbs(ComponentId::MainScreen, ScreenId::Main),
    mGui(),
    mScreenWidth(LvglProvider::GetDisplayWidth()),
    mScreenHeight(LvglProvider::GetDisplayHeight()){}

void MainScreenAbs::init(ComponentHostIfc& host){

    // init gui
    mGui.init();

    // calls onInit()
    ScreenAbs::init(host);
}

void MainScreenAbs::updateScreen(){
    ScreenAbs::updateScreen();
}

void MainScreenAbs::becomeEnabled(){
    mGui.show();
    ScreenAbs::becomeEnabled();
}

void MainScreenAbs::becomeDisabled(){
    ScreenAbs::becomeDisabled();
}

MainScreenGui::Objects& MainScreenAbs::gui(){
    return mGui.objects();
}

void MainScreenAbs::setBackgroundColor(uint32_t color){
    mGui.objects().root.setBgColor(lv_color_hex(color));
}

} // namespace Garbox