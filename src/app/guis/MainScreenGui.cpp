// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "MainScreenGui.h"

namespace Garbox {

MainScreenGui::MainScreenGui() : mObjects(){
    // nothing to do
}

void MainScreenGui::init(){
    mObjects.leftPane.setPad(8, 8, 8, 8);                                       // pad="8"
    mObjects.leftPane.setFlexGrow(1);                                           // flex-grow="1"
    mObjects.leftPane.setLayout(LV_LAYOUT_FLEX);                                // layout="flex"
    mObjects.leftPane.setFlexFlow(LV_FLEX_FLOW_COLUMN);                         // flex-flow="col"
    mObjects.leftPane.setScrollable(true);                                      // scrollable="true"
    mObjects.leftPane.setScrollDirection(LV_DIR_HOR);                           // scroll-dir="x"
    mObjects.settingPower.body.setLayout(LV_LAYOUT_FLEX);                       // layout="flex"
    mObjects.settingPower.body.setFlexFlow(LV_FLEX_FLOW_ROW);                   // flex-flow="row"
    mObjects.settingPower.body.setHeight(40);                                   // height="40"
    mObjects.settingPower.label.setText("Power");                               // text="Power"
    mObjects.settingPower.label.setFlexGrow(1);                                 // flex-grow="1"
    mObjects.settingPower.left.setText("<");                                    // text="<"
    mObjects.settingPower.left.setWidth(10);                                    // width="10px"
    mObjects.settingPower.left.setHidden(true);                                 // hidden="true"
    mObjects.settingPower.value.setText("On");                                  // text="On"
    mObjects.settingPower.value.setWidth(40);                                   // width="40px"
    mObjects.settingPower.value.setAlign(LV_ALIGN_RIGHT_MID, 0, 0);             // align="right"
    mObjects.settingPower.right.setText(">");                                   // text=">"
    mObjects.settingPower.right.setWidth(10);                                   // width="10px"
    mObjects.settingPower.right.setHidden(true);                                // hidden="true"
    mObjects.settingTarget.body.setLayout(LV_LAYOUT_FLEX);                      // layout="flex"
    mObjects.settingTarget.body.setFlexFlow(LV_FLEX_FLOW_ROW);                  // flex-flow="row"
    mObjects.settingTarget.body.setHeight(40);                                  // height="40"
    mObjects.settingTarget.label.setText("Target");                             // text="Target"
    mObjects.settingTarget.label.setFlexGrow(1);                                // flex-grow="1"
    mObjects.settingTarget.left.setText("<");                                   // text="<"
    mObjects.settingTarget.left.setWidth(10);                                   // width="10px"
    mObjects.settingTarget.left.setHidden(true);                                // hidden="true"
    mObjects.settingTarget.value.setText("30.0 °C");                            // text="30.0 °C"
    mObjects.settingTarget.value.setWidth(40);                                  // width="40px"
    mObjects.settingTarget.value.setAlign(LV_ALIGN_RIGHT_MID, 0, 0);            // align="right"
    mObjects.settingTarget.right.setText(">");                                  // text=">"
    mObjects.settingTarget.right.setWidth(10);                                  // width="10px"
    mObjects.settingTarget.right.setHidden(true);                               // hidden="true"
    mObjects.settingFan.body.setLayout(LV_LAYOUT_FLEX);                         // layout="flex"
    mObjects.settingFan.body.setFlexFlow(LV_FLEX_FLOW_ROW);                     // flex-flow="row"
    mObjects.settingFan.body.setHeight(40);                                     // height="40"
    mObjects.settingFan.label.setText("Fan");                                   // text="Fan"
    mObjects.settingFan.label.setFlexGrow(1);                                   // flex-grow="1"
    mObjects.settingFan.left.setText("<");                                      // text="<"
    mObjects.settingFan.left.setWidth(10);                                      // width="10px"
    mObjects.settingFan.left.setHidden(true);                                   // hidden="true"
    mObjects.settingFan.value.setText("auto");                                  // text="auto"
    mObjects.settingFan.value.setWidth(40);                                     // width="40px"
    mObjects.settingFan.value.setAlign(LV_ALIGN_RIGHT_MID, 0, 0);               // align="right"
    mObjects.settingFan.right.setText(">");                                     // text=">"
    mObjects.settingFan.right.setWidth(10);                                     // width="10px"
    mObjects.settingFan.right.setHidden(true);                                  // hidden="true"
    mObjects.settingHeater.body.setLayout(LV_LAYOUT_FLEX);                      // layout="flex"
    mObjects.settingHeater.body.setFlexFlow(LV_FLEX_FLOW_ROW);                  // flex-flow="row"
    mObjects.settingHeater.body.setHeight(40);                                  // height="40"
    mObjects.settingHeater.label.setText("Heater");                             // text="Heater"
    mObjects.settingHeater.label.setFlexGrow(1);                                // flex-grow="1"
    mObjects.settingHeater.left.setText("<");                                   // text="<"
    mObjects.settingHeater.left.setWidth(10);                                   // width="10px"
    mObjects.settingHeater.left.setHidden(true);                                // hidden="true"
    mObjects.settingHeater.value.setText("auto");                               // text="auto"
    mObjects.settingHeater.value.setWidth(40);                                  // width="40px"
    mObjects.settingHeater.value.setAlign(LV_ALIGN_RIGHT_MID, 0, 0);            // align="right"
    mObjects.settingHeater.right.setText(">");                                  // text=">"
    mObjects.settingHeater.right.setWidth(10);                                  // width="10px"
    mObjects.settingHeater.right.setHidden(true);                               // hidden="true"
    mObjects.settingCalibrate.body.setLayout(LV_LAYOUT_FLEX);                   // layout="flex"
    mObjects.settingCalibrate.body.setFlexFlow(LV_FLEX_FLOW_ROW);               // flex-flow="row"
    mObjects.settingCalibrate.body.setHeight(40);                               // height="40"
    mObjects.settingCalibrate.label.setText("Calibrate");                       // text="Calibrate"
    mObjects.settingCalibrate.label.setFlexGrow(1);                             // flex-grow="1"
    mObjects.settingCalibrate.left.setText("<");                                // text="<"
    mObjects.settingCalibrate.left.setWidth(10);                                // width="10px"
    mObjects.settingCalibrate.left.setHidden(true);                             // hidden="true"
    mObjects.settingCalibrate.value.setText("-");                               // text="-"
    mObjects.settingCalibrate.value.setWidth(40);                               // width="40px"
    mObjects.settingCalibrate.value.setAlign(LV_ALIGN_RIGHT_MID, 0, 0);         // align="right"
    mObjects.settingCalibrate.right.setText(">");                               // text=">"
    mObjects.settingCalibrate.right.setWidth(10);                               // width="10px"
    mObjects.settingCalibrate.right.setHidden(true);                            // hidden="true"
    mObjects.settingAdvanced.body.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects.settingAdvanced.body.setFlexFlow(LV_FLEX_FLOW_ROW);                // flex-flow="row"
    mObjects.settingAdvanced.body.setHeight(40);                                // height="40"
    mObjects.settingAdvanced.label.setText("Advanced");                         // text="Advanced"
    mObjects.settingAdvanced.label.setFlexGrow(1);                              // flex-grow="1"
    mObjects.settingAdvanced.left.setText("<");                                 // text="<"
    mObjects.settingAdvanced.left.setWidth(10);                                 // width="10px"
    mObjects.settingAdvanced.left.setHidden(true);                              // hidden="true"
    mObjects.settingAdvanced.value.setText("-");                                // text="-"
    mObjects.settingAdvanced.value.setWidth(40);                                // width="40px"
    mObjects.settingAdvanced.value.setAlign(LV_ALIGN_RIGHT_MID, 0, 0);          // align="right"
    mObjects.settingAdvanced.right.setText(">");                                // text=">"
    mObjects.settingAdvanced.right.setWidth(10);                                // width="10px"
    mObjects.settingAdvanced.right.setHidden(true);                             // hidden="true"
    mObjects.middlePane.setPad(8, 8, 8, 8);                                     // pad="8"
    mObjects.middlePane.setWidth(100);                                          // width="100px"
    mObjects.middlePane.setLayout(LV_LAYOUT_FLEX);                              // layout="flex"
    mObjects.middlePane.setFlexFlow(LV_FLEX_FLOW_COLUMN);                       // flex-flow="col"
    mObjects._namelessLabel_0.setText("Status");                                // text="Status"
    mObjects._namelessLabel_0.setHeight(30);                                    // height="30"
    mObjects._namelessLabel_0.setTextColor(lv_color_hex(0xcc88ff));             // text-color="#cc88ff"
    mObjects._namelessContainer_0.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_0.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="col"
    mObjects._namelessContainer_0.setFlexGrow(1);                               // flex-grow="1"
    mObjects._namelessInfoRow_0.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessInfoRow_0.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessInfoRow_0.body.setHeightContent();                        // height="auto"
    mObjects._namelessInfoRow_0.icon.setSource(&MainScreen_image_power_16px);   // src="power-16px"
    mObjects._namelessInfoRow_0.icon.setWidth(16);                              // width="16"
    mObjects._namelessInfoRow_0.icon.setHeight(16);                             // height="16"
    mObjects._namelessInfoRow_0.value.setText("Active");                        // text="Active"
    mObjects._namelessInfoRow_0.unit.setText("unit");                           // text="unit"
    mObjects._namelessInfoRow_0.unit.setHidden(false);                          // hidden="false"
    mObjects._namelessInfoRow_1.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessInfoRow_1.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessInfoRow_1.body.setHeightContent();                        // height="auto"
    mObjects._namelessInfoRow_1.icon.setSource(&MainScreen_image_fan_16px);     // src="fan-16px"
    mObjects._namelessInfoRow_1.icon.setWidth(16);                              // width="16"
    mObjects._namelessInfoRow_1.icon.setHeight(16);                             // height="16"
    mObjects._namelessInfoRow_1.value.setText("40.0");                          // text="40.0"
    mObjects._namelessInfoRow_1.unit.setText("%");                              // text="%"
    mObjects._namelessInfoRow_1.unit.setHidden(false);                          // hidden="false"
    mObjects._namelessInfoRow_2.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessInfoRow_2.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessInfoRow_2.body.setHeightContent();                        // height="auto"
    mObjects._namelessInfoRow_2.icon.setSource(&MainScreen_image_heat_16px);    // src="heat-16px"
    mObjects._namelessInfoRow_2.icon.setWidth(16);                              // width="16"
    mObjects._namelessInfoRow_2.icon.setHeight(16);                             // height="16"
    mObjects._namelessInfoRow_2.value.setText("23.0");                          // text="23.0"
    mObjects._namelessInfoRow_2.unit.setText("%");                              // text="%"
    mObjects._namelessInfoRow_2.unit.setHidden(false);                          // hidden="false"
    mObjects.rightPane.setPad(8, 8, 8, 8);                                      // pad="8"
    mObjects.rightPane.setWidth(100);                                           // width="100px"
    mObjects.rightPane.setLayout(LV_LAYOUT_FLEX);                               // layout="flex"
    mObjects.rightPane.setFlexFlow(LV_FLEX_FLOW_COLUMN);                        // flex-flow="col"
    mObjects._namelessLabel_1.setText("Sensors");                               // text="Sensors"
    mObjects._namelessLabel_1.setHeight(30);                                    // height="30"
    mObjects._namelessLabel_1.setTextColor(lv_color_hex(0x88ccff));             // text-color="#88ccff"
    mObjects._namelessContainer_1.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_1.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="col"
    mObjects._namelessContainer_1.setFlexGrow(1);                               // flex-grow="1"
    mObjects.fanRpm.body.setLayout(LV_LAYOUT_FLEX);                             // layout="flex"
    mObjects.fanRpm.body.setFlexFlow(LV_FLEX_FLOW_ROW);                         // flex-flow="row"
    mObjects.fanRpm.body.setHeightContent();                                    // height="auto"
    mObjects.fanRpm.icon.setSource(&MainScreen_image_fan_16px);                 // src="fan-16px"
    mObjects.fanRpm.icon.setWidth(16);                                          // width="16"
    mObjects.fanRpm.icon.setHeight(16);                                         // height="16"
    mObjects.fanRpm.value.setText("2000");                                      // text="2000"
    mObjects.fanRpm.unit.setText("RPM");                                        // text="RPM"
    mObjects.fanRpm.unit.setHidden(false);                                      // hidden="false"
    mObjects.temperature.body.setLayout(LV_LAYOUT_FLEX);                        // layout="flex"
    mObjects.temperature.body.setFlexFlow(LV_FLEX_FLOW_ROW);                    // flex-flow="row"
    mObjects.temperature.body.setHeightContent();                               // height="auto"
    mObjects.temperature.icon.setSource(&MainScreen_image_temperature_16px);    // src="temperature-16px"
    mObjects.temperature.icon.setWidth(16);                                     // width="16"
    mObjects.temperature.icon.setHeight(16);                                    // height="16"
    mObjects.temperature.value.setText("28.2");                                 // text="28.2"
    mObjects.temperature.unit.setText("°C");                                    // text="°C"
    mObjects.temperature.unit.setHidden(false);                                 // hidden="false"
    mObjects.humidity.body.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.humidity.body.setFlexFlow(LV_FLEX_FLOW_ROW);                       // flex-flow="row"
    mObjects.humidity.body.setHeightContent();                                  // height="auto"
    mObjects.humidity.icon.setSource(&MainScreen_image_humidity_16px);          // src="humidity-16px"
    mObjects.humidity.icon.setWidth(16);                                        // width="16"
    mObjects.humidity.icon.setHeight(16);                                       // height="16"
    mObjects.humidity.value.setText("56.3");                                    // text="56.3"
    mObjects.humidity.unit.setText("%");                                        // text="%"
    mObjects.humidity.unit.setHidden(false);                                    // hidden="false"
    mObjects.power.body.setLayout(LV_LAYOUT_FLEX);                              // layout="flex"
    mObjects.power.body.setFlexFlow(LV_FLEX_FLOW_ROW);                          // flex-flow="row"
    mObjects.power.body.setHeightContent();                                     // height="auto"
    mObjects.power.icon.setSource(&MainScreen_image_power_16px);                // src="power-16px"
    mObjects.power.icon.setWidth(16);                                           // width="16"
    mObjects.power.icon.setHeight(16);                                          // height="16"
    mObjects.power.value.setText("15.3");                                       // text="15.3"
    mObjects.power.unit.setText("W");                                           // text="W"
    mObjects.power.unit.setHidden(false);                                       // hidden="false"
}

void MainScreenGui::show(){
    lv_screen_load(mObjects.root.raw());
}

MainScreenGui::Objects& MainScreenGui::objects(){
    return mObjects;
}

// ============================================================
// === Component Structs ======================================
// ============================================================

MainScreenGui::SettingRow::SettingRow(LvObject& parent):
    body(nullptr),
    label(body),
    left(body),
    value(body),
    right(body){}

MainScreenGui::InfoRow::InfoRow(LvObject& parent):
    body(nullptr),
    icon(body),
    value(body),
    unit(body){}

// ============================================================
// === Objects Struct =========================================
// ============================================================

MainScreenGui::Objects::Objects():
    root(nullptr),
    wrapper(root),
    leftPane(wrapper),
    settingPower(leftPane),
    settingTarget(leftPane),
    settingFan(leftPane),
    settingHeater(leftPane),
    settingCalibrate(leftPane),
    settingAdvanced(leftPane),
    middlePane(wrapper),
    _namelessLabel_0(middlePane),
    _namelessContainer_0(middlePane),
    _namelessInfoRow_0(_namelessContainer_0),
    _namelessInfoRow_1(_namelessContainer_0),
    _namelessInfoRow_2(_namelessContainer_0),
    rightPane(wrapper),
    _namelessLabel_1(rightPane),
    _namelessContainer_1(rightPane),
    fanRpm(_namelessContainer_1),
    temperature(_namelessContainer_1),
    humidity(_namelessContainer_1),
    power(_namelessContainer_1){}

} // namespace

