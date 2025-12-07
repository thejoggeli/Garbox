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
    mObjects.root.setBorder(0, lv_color_hex(0x000000));
    mObjects.root.setRadius(0);
    mObjects.root.setPad(0, 0, 0, 0);
    mObjects.root.setBgOpa(LV_OPA_COVER);
    mObjects.root.setScrollable(false);

    // generated from xml
    mObjects.root.setBgColor(lv_color_hex(0x000000));                           // bg-color="#000000"
    mObjects.root.setTextColor(lv_color_hex(0xffffff));                         // text-color="#ffffff"
    mObjects._namelessContainer_0.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_0.setFlexFlow(LV_FLEX_FLOW_ROW);                // flex-flow="row"
    mObjects._namelessContainer_0.setWidth(LV_PCT(100));                        // width="100%"
    mObjects._namelessContainer_0.setHeight(LV_PCT(100));                       // height="100%"
    mObjects._namelessContainer_0.setBorderColor(lv_color_hex(0xffffff));       // border-color="#ffffff"
    mObjects._namelessContainer_0.setBorderWidth(1);                            // border-width="1px"
    mObjects._namelessContainer_0.setBorderSide(LV_BORDER_SIDE_FULL);           // border-side="full"
    mObjects._namelessContainer_1.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_1.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_1.setFlexAlign(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START); // flex-align="start start start"
    mObjects._namelessContainer_1.setFlexGrow(1);                               // flex-grow="1"
    mObjects._namelessContainer_1.setHeight(LV_PCT(100));                       // height="100%"
    mObjects._namelessContainer_1.setPad(4, 4, 4, 4);                           // pad="4px"
    mObjects._namelessContainer_1.setPadGap(4);                                 // pad-gap="4px"
    mObjects.settingPower.body.setLayout(LV_LAYOUT_FLEX);                       // layout="flex"
    mObjects.settingPower.body.setFlexFlow(LV_FLEX_FLOW_ROW);                   // flex-flow="row"
    mObjects.settingPower.body.setWidthContent();                               // width="content"
    mObjects.settingPower.body.setHeightContent();                              // height="content"
    mObjects.settingPower.body.setBgColor(lv_color_hex(0x880088));              // bg-color="#880088"
    mObjects.settingPower.body.setBgOpa(255);                                   // bg-opa="100%"
    mObjects.settingPower.label.setText("Power");                               // text="Power"
    mObjects.settingPower.label.setTextAlign(LV_TEXT_ALIGN_LEFT);               // text-align="left"
    mObjects.settingPower.label.setWidthContent();                              // width="content"
    mObjects.settingPower.value.setText("On");                                  // text="On"
    mObjects.settingPower.value.setTextAlign(LV_TEXT_ALIGN_CENTER);             // text-align="center"
    mObjects.settingPower.value.setWidthContent();                              // width="content"
    mObjects.settingTarget.body.setLayout(LV_LAYOUT_FLEX);                      // layout="flex"
    mObjects.settingTarget.body.setFlexFlow(LV_FLEX_FLOW_ROW);                  // flex-flow="row"
    mObjects.settingTarget.body.setWidthContent();                              // width="content"
    mObjects.settingTarget.body.setHeightContent();                             // height="content"
    mObjects.settingTarget.body.setBgColor(lv_color_hex(0x008888));             // bg-color="#008888"
    mObjects.settingTarget.body.setBgOpa(255);                                  // bg-opa="100%"
    mObjects.settingTarget.label.setText("Target");                             // text="Target"
    mObjects.settingTarget.label.setTextAlign(LV_TEXT_ALIGN_LEFT);              // text-align="left"
    mObjects.settingTarget.label.setWidthContent();                             // width="content"
    mObjects.settingTarget.value.setText("30.0°C");                             // text="30.0°C"
    mObjects.settingTarget.value.setTextAlign(LV_TEXT_ALIGN_CENTER);            // text-align="center"
    mObjects.settingTarget.value.setWidthContent();                             // width="content"
    mObjects.settingFan.body.setLayout(LV_LAYOUT_FLEX);                         // layout="flex"
    mObjects.settingFan.body.setFlexFlow(LV_FLEX_FLOW_ROW);                     // flex-flow="row"
    mObjects.settingFan.body.setWidthContent();                                 // width="content"
    mObjects.settingFan.body.setHeightContent();                                // height="content"
    mObjects.settingFan.body.setBgColor(lv_color_hex(0x880000));                // bg-color="#880000"
    mObjects.settingFan.body.setBgOpa(255);                                     // bg-opa="100%"
    mObjects.settingFan.label.setText("Fan");                                   // text="Fan"
    mObjects.settingFan.label.setTextAlign(LV_TEXT_ALIGN_LEFT);                 // text-align="left"
    mObjects.settingFan.label.setWidthContent();                                // width="content"
    mObjects.settingFan.value.setText("auto");                                  // text="auto"
    mObjects.settingFan.value.setTextAlign(LV_TEXT_ALIGN_CENTER);               // text-align="center"
    mObjects.settingFan.value.setWidthContent();                                // width="content"
    mObjects.settingHeater.body.setLayout(LV_LAYOUT_FLEX);                      // layout="flex"
    mObjects.settingHeater.body.setFlexFlow(LV_FLEX_FLOW_ROW);                  // flex-flow="row"
    mObjects.settingHeater.body.setWidthContent();                              // width="content"
    mObjects.settingHeater.body.setHeightContent();                             // height="content"
    mObjects.settingHeater.body.setBgColor(lv_color_hex(0x000088));             // bg-color="#000088"
    mObjects.settingHeater.body.setBgOpa(255);                                  // bg-opa="100%"
    mObjects.settingHeater.label.setText("Heater");                             // text="Heater"
    mObjects.settingHeater.label.setTextAlign(LV_TEXT_ALIGN_LEFT);              // text-align="left"
    mObjects.settingHeater.label.setWidthContent();                             // width="content"
    mObjects.settingHeater.value.setText("auto");                               // text="auto"
    mObjects.settingHeater.value.setTextAlign(LV_TEXT_ALIGN_CENTER);            // text-align="center"
    mObjects.settingHeater.value.setWidthContent();                             // width="content"
    mObjects.settingBrightness.body.setLayout(LV_LAYOUT_FLEX);                  // layout="flex"
    mObjects.settingBrightness.body.setFlexFlow(LV_FLEX_FLOW_ROW);              // flex-flow="row"
    mObjects.settingBrightness.body.setWidthContent();                          // width="content"
    mObjects.settingBrightness.body.setHeightContent();                         // height="content"
    mObjects.settingBrightness.body.setBgColor(lv_color_hex(0x008800));         // bg-color="#008800"
    mObjects.settingBrightness.body.setBgOpa(255);                              // bg-opa="100%"
    mObjects.settingBrightness.label.setText("Brightness");                     // text="Brightness"
    mObjects.settingBrightness.label.setTextAlign(LV_TEXT_ALIGN_LEFT);          // text-align="left"
    mObjects.settingBrightness.label.setWidthContent();                         // width="content"
    mObjects.settingBrightness.value.setText("-");                              // text="-"
    mObjects.settingBrightness.value.setTextAlign(LV_TEXT_ALIGN_CENTER);        // text-align="center"
    mObjects.settingBrightness.value.setWidthContent();                         // width="content"
    mObjects.settingCalibrate.body.setLayout(LV_LAYOUT_FLEX);                   // layout="flex"
    mObjects.settingCalibrate.body.setFlexFlow(LV_FLEX_FLOW_ROW);               // flex-flow="row"
    mObjects.settingCalibrate.body.setWidthContent();                           // width="content"
    mObjects.settingCalibrate.body.setHeightContent();                          // height="content"
    mObjects.settingCalibrate.body.setBgColor(lv_color_hex(0x888800));          // bg-color="#888800"
    mObjects.settingCalibrate.body.setBgOpa(255);                               // bg-opa="100%"
    mObjects.settingCalibrate.label.setText("Calibrate");                       // text="Calibrate"
    mObjects.settingCalibrate.label.setTextAlign(LV_TEXT_ALIGN_LEFT);           // text-align="left"
    mObjects.settingCalibrate.label.setWidthContent();                          // width="content"
    mObjects.settingCalibrate.value.setText("-");                               // text="-"
    mObjects.settingCalibrate.value.setTextAlign(LV_TEXT_ALIGN_CENTER);         // text-align="center"
    mObjects.settingCalibrate.value.setWidthContent();                          // width="content"
    mObjects.settingAdvanced.body.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects.settingAdvanced.body.setFlexFlow(LV_FLEX_FLOW_ROW);                // flex-flow="row"
    mObjects.settingAdvanced.body.setWidthContent();                            // width="content"
    mObjects.settingAdvanced.body.setHeightContent();                           // height="content"
    mObjects.settingAdvanced.body.setBgColor(lv_color_hex(0x880088));           // bg-color="#880088"
    mObjects.settingAdvanced.body.setBgOpa(255);                                // bg-opa="100%"
    mObjects.settingAdvanced.label.setText("Advanced");                         // text="Advanced"
    mObjects.settingAdvanced.label.setTextAlign(LV_TEXT_ALIGN_LEFT);            // text-align="left"
    mObjects.settingAdvanced.label.setWidthContent();                           // width="content"
    mObjects.settingAdvanced.value.setText("-");                                // text="-"
    mObjects.settingAdvanced.value.setTextAlign(LV_TEXT_ALIGN_CENTER);          // text-align="center"
    mObjects.settingAdvanced.value.setWidthContent();                           // width="content"
    mObjects._namelessContainer_2.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_2.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_2.setFlexAlign(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="start center center"
    mObjects._namelessContainer_2.setWidth(92);                                 // width="92px"
    mObjects._namelessContainer_2.setHeight(LV_PCT(100));                       // height="100%"
    mObjects._namelessContainer_2.setPad(4, 4, 4, 4);                           // pad="4px"
    mObjects._namelessContainer_2.setPadGap(8);                                 // pad-gap="8px"
    mObjects._namelessContainer_2.setBorderColor(lv_color_hex(0xffffff));       // border-color="#ffffff"
    mObjects._namelessContainer_2.setBorderWidth(1);                            // border-width="1px"
    mObjects._namelessContainer_2.setBorderSide(LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT); // border-side="left right"
    mObjects._namelessContainer_2.setBorderOpa(255);                            // border-opa="100%"
    mObjects._namelessLabel_0.setText("Setpoint");                              // text="Setpoint"
    mObjects._namelessLabel_0.setTextAlign(LV_TEXT_ALIGN_CENTER);               // text-align="center"
    mObjects.statusEngine.body.setLayout(LV_LAYOUT_FLEX);                       // layout="flex"
    mObjects.statusEngine.body.setFlexFlow(LV_FLEX_FLOW_ROW);                   // flex-flow="row"
    mObjects.statusEngine.body.setHeightContent();                              // height="content"
    mObjects.statusEngine.body.setWidth(LV_PCT(100));                           // width="100%"
    mObjects.statusEngine.icon.setSource(&MainScreen_image_power_16px);         // src="power-16px"
    mObjects.statusEngine.icon.setWidth(16);                                    // width="16"
    mObjects.statusEngine.icon.setHeight(16);                                   // height="16"
    mObjects.statusEngine.value.setText("");                                    // text=""
    mObjects.statusEngine.value.setPadLeft(4);                                  // pad-left="4px"
    mObjects.statusEngine.value.setPadRight(4);                                 // pad-right="4px"
    mObjects.statusEngine.unit.setText("unit");                                 // text="unit"
    mObjects.statusEngine.unit.setHidden(true);                                 // hidden="true"
    mObjects.statusEngine.unit.setTextAlign(LV_TEXT_ALIGN_RIGHT);               // text-align="right"
    mObjects.statusEngine.unit.setFlexGrow(1);                                  // flex-grow="1"
    mObjects.statusTemperature.body.setLayout(LV_LAYOUT_FLEX);                  // layout="flex"
    mObjects.statusTemperature.body.setFlexFlow(LV_FLEX_FLOW_ROW);              // flex-flow="row"
    mObjects.statusTemperature.body.setHeightContent();                         // height="content"
    mObjects.statusTemperature.body.setWidth(LV_PCT(100));                      // width="100%"
    mObjects.statusTemperature.icon.setSource(&MainScreen_image_temperature_16px); // src="temperature-16px"
    mObjects.statusTemperature.icon.setWidth(16);                               // width="16"
    mObjects.statusTemperature.icon.setHeight(16);                              // height="16"
    mObjects.statusTemperature.value.setText("");                               // text=""
    mObjects.statusTemperature.value.setPadLeft(4);                             // pad-left="4px"
    mObjects.statusTemperature.value.setPadRight(4);                            // pad-right="4px"
    mObjects.statusTemperature.unit.setText("°C");                              // text="°C"
    mObjects.statusTemperature.unit.setHidden(false);                           // hidden="false"
    mObjects.statusTemperature.unit.setTextAlign(LV_TEXT_ALIGN_RIGHT);          // text-align="right"
    mObjects.statusTemperature.unit.setFlexGrow(1);                             // flex-grow="1"
    mObjects.statusHeat.body.setLayout(LV_LAYOUT_FLEX);                         // layout="flex"
    mObjects.statusHeat.body.setFlexFlow(LV_FLEX_FLOW_ROW);                     // flex-flow="row"
    mObjects.statusHeat.body.setHeightContent();                                // height="content"
    mObjects.statusHeat.body.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.statusHeat.icon.setSource(&MainScreen_image_heat_16px);            // src="heat-16px"
    mObjects.statusHeat.icon.setWidth(16);                                      // width="16"
    mObjects.statusHeat.icon.setHeight(16);                                     // height="16"
    mObjects.statusHeat.value.setText("");                                      // text=""
    mObjects.statusHeat.value.setPadLeft(4);                                    // pad-left="4px"
    mObjects.statusHeat.value.setPadRight(4);                                   // pad-right="4px"
    mObjects.statusHeat.unit.setText("%");                                      // text="%"
    mObjects.statusHeat.unit.setHidden(false);                                  // hidden="false"
    mObjects.statusHeat.unit.setTextAlign(LV_TEXT_ALIGN_RIGHT);                 // text-align="right"
    mObjects.statusHeat.unit.setFlexGrow(1);                                    // flex-grow="1"
    mObjects.statusFan.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.statusFan.body.setFlexFlow(LV_FLEX_FLOW_ROW);                      // flex-flow="row"
    mObjects.statusFan.body.setHeightContent();                                 // height="content"
    mObjects.statusFan.body.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.statusFan.icon.setSource(&MainScreen_image_fan_16px);              // src="fan-16px"
    mObjects.statusFan.icon.setWidth(16);                                       // width="16"
    mObjects.statusFan.icon.setHeight(16);                                      // height="16"
    mObjects.statusFan.value.setText("");                                       // text=""
    mObjects.statusFan.value.setPadLeft(4);                                     // pad-left="4px"
    mObjects.statusFan.value.setPadRight(4);                                    // pad-right="4px"
    mObjects.statusFan.unit.setText("%");                                       // text="%"
    mObjects.statusFan.unit.setHidden(false);                                   // hidden="false"
    mObjects.statusFan.unit.setTextAlign(LV_TEXT_ALIGN_RIGHT);                  // text-align="right"
    mObjects.statusFan.unit.setFlexGrow(1);                                     // flex-grow="1"
    mObjects._namelessContainer_3.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_3.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_3.setFlexAlign(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="start center center"
    mObjects._namelessContainer_3.setWidth(90);                                 // width="90px"
    mObjects._namelessContainer_3.setHeight(LV_PCT(100));                       // height="100%"
    mObjects._namelessContainer_3.setPad(4, 4, 4, 4);                           // pad="4px"
    mObjects._namelessContainer_3.setPadGap(8);                                 // pad-gap="8px"
    mObjects._namelessLabel_1.setText("Sensors");                               // text="Sensors"
    mObjects._namelessLabel_1.setTextAlign(LV_TEXT_ALIGN_CENTER);               // text-align="center"
    mObjects.sensorPower.body.setLayout(LV_LAYOUT_FLEX);                        // layout="flex"
    mObjects.sensorPower.body.setFlexFlow(LV_FLEX_FLOW_ROW);                    // flex-flow="row"
    mObjects.sensorPower.body.setHeightContent();                               // height="content"
    mObjects.sensorPower.body.setWidth(LV_PCT(100));                            // width="100%"
    mObjects.sensorPower.icon.setSource(&MainScreen_image_power_16px);          // src="power-16px"
    mObjects.sensorPower.icon.setWidth(16);                                     // width="16"
    mObjects.sensorPower.icon.setHeight(16);                                    // height="16"
    mObjects.sensorPower.value.setText("");                                     // text=""
    mObjects.sensorPower.value.setPadLeft(4);                                   // pad-left="4px"
    mObjects.sensorPower.value.setPadRight(4);                                  // pad-right="4px"
    mObjects.sensorPower.unit.setText("W");                                     // text="W"
    mObjects.sensorPower.unit.setHidden(false);                                 // hidden="false"
    mObjects.sensorPower.unit.setTextAlign(LV_TEXT_ALIGN_RIGHT);                // text-align="right"
    mObjects.sensorPower.unit.setFlexGrow(1);                                   // flex-grow="1"
    mObjects.sensorTemperature.body.setLayout(LV_LAYOUT_FLEX);                  // layout="flex"
    mObjects.sensorTemperature.body.setFlexFlow(LV_FLEX_FLOW_ROW);              // flex-flow="row"
    mObjects.sensorTemperature.body.setHeightContent();                         // height="content"
    mObjects.sensorTemperature.body.setWidth(LV_PCT(100));                      // width="100%"
    mObjects.sensorTemperature.icon.setSource(&MainScreen_image_temperature_16px); // src="temperature-16px"
    mObjects.sensorTemperature.icon.setWidth(16);                               // width="16"
    mObjects.sensorTemperature.icon.setHeight(16);                              // height="16"
    mObjects.sensorTemperature.value.setText("");                               // text=""
    mObjects.sensorTemperature.value.setPadLeft(4);                             // pad-left="4px"
    mObjects.sensorTemperature.value.setPadRight(4);                            // pad-right="4px"
    mObjects.sensorTemperature.unit.setText("°C");                              // text="°C"
    mObjects.sensorTemperature.unit.setHidden(false);                           // hidden="false"
    mObjects.sensorTemperature.unit.setTextAlign(LV_TEXT_ALIGN_RIGHT);          // text-align="right"
    mObjects.sensorTemperature.unit.setFlexGrow(1);                             // flex-grow="1"
    mObjects.sensorHumidity.body.setLayout(LV_LAYOUT_FLEX);                     // layout="flex"
    mObjects.sensorHumidity.body.setFlexFlow(LV_FLEX_FLOW_ROW);                 // flex-flow="row"
    mObjects.sensorHumidity.body.setHeightContent();                            // height="content"
    mObjects.sensorHumidity.body.setWidth(LV_PCT(100));                         // width="100%"
    mObjects.sensorHumidity.icon.setSource(&MainScreen_image_humidity_16px);    // src="humidity-16px"
    mObjects.sensorHumidity.icon.setWidth(16);                                  // width="16"
    mObjects.sensorHumidity.icon.setHeight(16);                                 // height="16"
    mObjects.sensorHumidity.value.setText("");                                  // text=""
    mObjects.sensorHumidity.value.setPadLeft(4);                                // pad-left="4px"
    mObjects.sensorHumidity.value.setPadRight(4);                               // pad-right="4px"
    mObjects.sensorHumidity.unit.setText("%");                                  // text="%"
    mObjects.sensorHumidity.unit.setHidden(false);                              // hidden="false"
    mObjects.sensorHumidity.unit.setTextAlign(LV_TEXT_ALIGN_RIGHT);             // text-align="right"
    mObjects.sensorHumidity.unit.setFlexGrow(1);                                // flex-grow="1"
    mObjects.sensorFan.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.sensorFan.body.setFlexFlow(LV_FLEX_FLOW_ROW);                      // flex-flow="row"
    mObjects.sensorFan.body.setHeightContent();                                 // height="content"
    mObjects.sensorFan.body.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.sensorFan.icon.setSource(&MainScreen_image_fan_16px);              // src="fan-16px"
    mObjects.sensorFan.icon.setWidth(16);                                       // width="16"
    mObjects.sensorFan.icon.setHeight(16);                                      // height="16"
    mObjects.sensorFan.value.setText("");                                       // text=""
    mObjects.sensorFan.value.setPadLeft(4);                                     // pad-left="4px"
    mObjects.sensorFan.value.setPadRight(4);                                    // pad-right="4px"
    mObjects.sensorFan.value.setFlexGrow(1);                                    // flex-grow="1"
    mObjects.sensorFan.unit.setSource(&MainScreen_image_rpm_16px);              // src="rpm-16px"
    mObjects.sensorFan.unit.setHidden(false);                                   // hidden="false"
    mObjects.sensorFan.unit.setWidth(16);                                       // width="16"
    mObjects.sensorFan.unit.setHeight(16);                                      // height="16"
}

void MainScreenGui::show(){
    mObjects.root.setScreen();
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
    value(body){}

MainScreenGui::InfoRowUnitText::InfoRowUnitText(LvObject& parent):
    body(parent),
    icon(body),
    value(body),
    unit(body){}

MainScreenGui::InfoRowUnitIcon::InfoRowUnitIcon(LvObject& parent):
    body(parent),
    icon(body),
    value(body),
    unit(body){}

// ============================================================
// === Objects Struct =========================================
// ============================================================

MainScreenGui::Objects::Objects():
    root(),
    _namelessContainer_0(root),
    _namelessContainer_1(_namelessContainer_0),
    settingPower(_namelessContainer_1),
    settingTarget(_namelessContainer_1),
    settingFan(_namelessContainer_1),
    settingHeater(_namelessContainer_1),
    settingBrightness(_namelessContainer_1),
    settingCalibrate(_namelessContainer_1),
    settingAdvanced(_namelessContainer_1),
    _namelessContainer_2(_namelessContainer_0),
    _namelessLabel_0(_namelessContainer_2),
    statusEngine(_namelessContainer_2),
    statusTemperature(_namelessContainer_2),
    statusHeat(_namelessContainer_2),
    statusFan(_namelessContainer_2),
    _namelessContainer_3(_namelessContainer_0),
    _namelessLabel_1(_namelessContainer_3),
    sensorPower(_namelessContainer_3),
    sensorTemperature(_namelessContainer_3),
    sensorHumidity(_namelessContainer_3),
    sensorFan(_namelessContainer_3){}

} // namespace

