// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "MainScreenGui.h"

namespace Garbox {

MainScreenGui::MainScreenGui() : mObjects(){
    // nothing to do
}

void MainScreenGui::init(){
    
    // initialize root
    mObjects.root.setHidden(true);
    mObjects.root.setBorder(0, lv_color_hex(0x000000));
    mObjects.root.setRadius(0);
    mObjects.root.setPad(0, 0, 0, 0);
    mObjects.root.setBgOpacity(LV_OPA_COVER);
    mObjects.root.setScrollable(false);

    // generated from xml
    mObjects.root.setBgColor(lv_color_hex(0x000000));                           // bg-color="#000000"
    mObjects.root.setTextColor(lv_color_hex(0xffffff));                         // text-color="#ffffff"
    mObjects.root.setLayout(LV_LAYOUT_FLEX);                                    // layout="flex"
    mObjects.root.setFlexFlow(LV_FLEX_FLOW_COLUMN);                             // flex-flow="column"
    mObjects.settingPower.body.setLayout(LV_LAYOUT_FLEX);                       // layout="flex"
    mObjects.settingPower.body.setFlexFlow(LV_FLEX_FLOW_ROW);                   // flex-flow="row"
    mObjects.settingPower.body.setBgColor(lv_color_hex(0xff00ff));              // bg-color="#ff00ff"
    mObjects.settingPower.body.setBgOpacity(255);                               // bg-opacity="100%"
    mObjects.settingPower.label.setText("Power");                               // text="Power"
    mObjects.settingPower.label.setTextAlign(LV_TEXT_ALIGN_LEFT);               // text-align="left"
    mObjects.settingPower.label.setPadRight(8);                                 // pad-right="8px"
    mObjects.settingPower.left.setText("<");                                    // text="<"
    mObjects.settingPower.left.setTextAlign(LV_TEXT_ALIGN_CENTER);              // text-align="center"
    mObjects.settingPower.left.setWidth(16);                                    // width="16px"
    mObjects.settingPower.left.setHidden(true);                                 // hidden="true"
    mObjects.settingPower.value.setText("On");                                  // text="On"
    mObjects.settingPower.value.setTextAlign(LV_TEXT_ALIGN_CENTER);             // text-align="center"
    mObjects.settingPower.value.setWidth(80);                                   // width="80px"
    mObjects.settingPower.right.setText(">");                                   // text=">"
    mObjects.settingPower.right.setTextAlign(LV_TEXT_ALIGN_CENTER);             // text-align="center"
    mObjects.settingPower.right.setWidth(16);                                   // width="16px"
    mObjects.settingPower.right.setHidden(true);                                // hidden="true"
    mObjects.settingTarget.body.setLayout(LV_LAYOUT_FLEX);                      // layout="flex"
    mObjects.settingTarget.body.setFlexFlow(LV_FLEX_FLOW_ROW);                  // flex-flow="row"
    mObjects.settingTarget.body.setBgColor(lv_color_hex(0x00ffff));             // bg-color="#00ffff"
    mObjects.settingTarget.body.setBgOpacity(255);                              // bg-opacity="100%"
    mObjects.settingTarget.label.setText("Target");                             // text="Target"
    mObjects.settingTarget.label.setTextAlign(LV_TEXT_ALIGN_LEFT);              // text-align="left"
    mObjects.settingTarget.label.setPadRight(8);                                // pad-right="8px"
    mObjects.settingTarget.left.setText("<");                                   // text="<"
    mObjects.settingTarget.left.setTextAlign(LV_TEXT_ALIGN_CENTER);             // text-align="center"
    mObjects.settingTarget.left.setWidth(16);                                   // width="16px"
    mObjects.settingTarget.left.setHidden(true);                                // hidden="true"
    mObjects.settingTarget.value.setText("30.0°C");                             // text="30.0°C"
    mObjects.settingTarget.value.setTextAlign(LV_TEXT_ALIGN_CENTER);            // text-align="center"
    mObjects.settingTarget.value.setWidth(80);                                  // width="80px"
    mObjects.settingTarget.right.setText(">");                                  // text=">"
    mObjects.settingTarget.right.setTextAlign(LV_TEXT_ALIGN_CENTER);            // text-align="center"
    mObjects.settingTarget.right.setWidth(16);                                  // width="16px"
    mObjects.settingTarget.right.setHidden(true);                               // hidden="true"
}

void MainScreenGui::show(){
    mObjects.root.setHidden(false);
    mObjects.root.setScreen();
}

void MainScreenGui::hide(){
    mObjects.root.setHidden(true);
}

MainScreenGui::Objects& MainScreenGui::objects(){
    return mObjects;
}

// ============================================================
// === Component Structs ======================================
// ============================================================

MainScreenGui::SettingRow::SettingRow(LvObject& parent):
    body(parent),
    label(body),
    left(body),
    value(body),
    right(body){}

// ============================================================
// === Objects Struct =========================================
// ============================================================

MainScreenGui::Objects::Objects():
    root(),
    settingPower(root),
    settingTarget(root){}

} // namespace

