// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "SimpleScreenGui.h"

namespace Garbox {

SimpleScreenGui::SimpleScreenGui() : mObjects(){
    // nothing to do
}

void SimpleScreenGui::init(){
    
    // initialize root
    mObjects.root.setBorder(0, lv_color_hex(0x000000));
    mObjects.root.setRadius(0);
    mObjects.root.setPad(0, 0, 0, 0);
    mObjects.root.setBgOpa(LV_OPA_COVER);
    mObjects.root.setScrollable(false);

    // generated from xml
    mObjects.root.setBgColor(lv_color_hex(0x070b0e));                           // bg-color="#070B0E"
    mObjects.root.setTextColor(lv_color_hex(0xffffff));                         // text-color="#FFFFFF"
    mObjects.wrapper.setLayout(LV_LAYOUT_FLEX);                                 // layout="flex"
    mObjects.wrapper.setFlexFlow(LV_FLEX_FLOW_COLUMN);                          // flex-flow="column"
    mObjects.wrapper.setWidth(LV_PCT(100));                                     // width="100%"
    mObjects.wrapper.setHeight(220);                                            // height="220px"
    mObjects.wrapper.setPadLeft(8);                                             // pad-left="8px"
    mObjects.wrapper.setPadRight(8);                                            // pad-right="8px"
    mObjects.wrapper.setPadTop(7);                                              // pad-top="7px"
    mObjects.wrapper.setPadBottom(5);                                           // pad-bottom="5px"
    mObjects.wrapper.setPadGap(6);                                              // pad-gap="6px"
    mObjects.header.setWidth(LV_PCT(100));                                      // width="100%"
    mObjects.header.setHeight(25);                                              // height="25px"
    mObjects.header.setLayout(LV_LAYOUT_FLEX);                                  // layout="flex"
    mObjects.header.setFlexFlow(LV_FLEX_FLOW_ROW);                              // flex-flow="row"
    mObjects.header.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.statusPill.setWidth(LV_PCT(100));                                  // width="100%"
    mObjects.statusPill.setHeight(25);                                          // height="25px"
    mObjects.statusPill.setLayout(LV_LAYOUT_FLEX);                              // layout="flex"
    mObjects.statusPill.setFlexFlow(LV_FLEX_FLOW_ROW);                          // flex-flow="row"
    mObjects.statusPill.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.statusPill.setPadGap(5);                                           // pad-gap="5px"
    mObjects.statusPill.setBgColor(lv_color_hex(0x16241b));                     // bg-color="#16241B"
    mObjects.statusPill.setBgOpa(255);                                          // bg-opa="100%"
    mObjects.statusPill.setRadius(12);                                          // radius="12px"
    mObjects.statusIcon.setSource(&SimpleScreen_image_power16);                 // src="power16"
    mObjects.statusIcon.setWidth(16);                                           // width="16px"
    mObjects.statusIcon.setHeight(16);                                          // height="16px"
    mObjects.statusText.setText("CONTROL OFF");                                 // text="CONTROL OFF"
    mObjects.content.setWidth(LV_PCT(100));                                     // width="100%"
    mObjects.content.setFlexGrow(1);                                            // flex-grow="1"
    mObjects.content.setLayout(LV_LAYOUT_FLEX);                                 // layout="flex"
    mObjects.content.setFlexFlow(LV_FLEX_FLOW_ROW);                             // flex-flow="row"
    mObjects.content.setPadGap(7);                                              // pad-gap="7px"
    mObjects.gaugePanel.setWidth(194);                                          // width="194px"
    mObjects.gaugePanel.setHeight(LV_PCT(100));                                 // height="100%"
    mObjects.gaugePanel.setBgColor(lv_color_hex(0x0d141a));                     // bg-color="#0D141A"
    mObjects.gaugePanel.setBgOpa(255);                                          // bg-opa="100%"
    mObjects.gaugePanel.setRadius(12);                                          // radius="12px"
    mObjects.gaugePanel.setBorderColor(lv_color_hex(0x26343d));                 // border-color="#26343D"
    mObjects.gaugePanel.setBorderWidth(1);                                      // border-width="1px"
    mObjects.gaugePanel.setBorderSide(LV_BORDER_SIDE_FULL);                     // border-side="full"
    mObjects.arcContainer.setAlign(LV_ALIGN_CENTER, 0, 0);                      // align="center"
    mObjects.arcContainer.setWidth(158);                                        // width="158px"
    mObjects.arcContainer.setHeight(158);                                       // height="158px"
    mObjects.arcBg.setAlign(LV_ALIGN_CENTER, 0, 0);                             // align="center"
    mObjects.arcBg.setWidth(158);                                               // width="158px"
    mObjects.arcBg.setHeight(158);                                              // height="158px"
    mObjects.arcBg.setArcBgWidth(13);                                           // arc-bg-width="13px"
    mObjects.arcBg.setArcBgAngles(0, 359);                                      // arc-bg-angles="0 359"
    mObjects.arcBg.setArcBgColor(lv_color_hex(0x263138));                       // arc-bg-color="#263138"
    mObjects.arcBg.setArcOpa(0);                                                // arc-opa="0%"
    mObjects.arcBg.setArcBgOpa(255);                                            // arc-bg-opa="100%"
    mObjects.arcBg.setArcBgRounded(true);                                       // arc-bg-rounded="1"
    mObjects.arcMeasured.setAlign(LV_ALIGN_CENTER, 0, 0);                       // align="center"
    mObjects.arcMeasured.setWidth(158);                                         // width="158px"
    mObjects.arcMeasured.setHeight(158);                                        // height="158px"
    mObjects.arcMeasured.setArcWidth(13);                                       // arc-width="13px"
    mObjects.arcMeasured.setArcAngles(0, 60);                                   // arc-angles="0 60"
    mObjects.arcMeasured.setArcColor(lv_color_hex(0x8ab4f4));                   // arc-color="#8AB4F4"
    mObjects.arcMeasured.setArcOpa(255);                                        // arc-opa="100%"
    mObjects.arcMeasured.setArcBgOpa(0);                                        // arc-bg-opa="0%"
    mObjects.arcMeasured.setArcRounded(true);                                   // arc-rounded="1"
    mObjects.arcTarget.setAlign(LV_ALIGN_CENTER, 0, 0);                         // align="center"
    mObjects.arcTarget.setWidth(136);                                           // width="136px"
    mObjects.arcTarget.setHeight(136);                                          // height="136px"
    mObjects.arcTarget.setArcWidth(4);                                          // arc-width="4px"
    mObjects.arcTarget.setArcAngles(0, 120);                                    // arc-angles="0 120"
    mObjects.arcTarget.setArcColor(lv_color_hex(0xe3c567));                     // arc-color="#E3C567"
    mObjects.arcTarget.setArcOpa(255);                                          // arc-opa="100%"
    mObjects.arcTarget.setArcBgOpa(0);                                          // arc-bg-opa="0%"
    mObjects.arcTarget.setArcRounded(true);                                     // arc-rounded="1"
    mObjects._namelessContainer_0.setAlign(LV_ALIGN_CENTER, 0, 0);              // align="center"
    mObjects._namelessContainer_0.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_0.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_0.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects._namelessContainer_0.setWidth(120);                                // width="120px"
    mObjects.measuredLabel.setText("CHAMBER");                                  // text="CHAMBER"
    mObjects.measuredLabel.setTextColor(lv_color_hex(0x778791));                // text-color="#778791"
    mObjects._namelessContainer_1.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_1.setFlexFlow(LV_FLEX_FLOW_ROW);                // flex-flow="row"
    mObjects._namelessContainer_1.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END); // flex-align="center end end"
    mObjects._namelessContainer_1.setWidthContent();                            // width="content"
    mObjects._namelessContainer_1.setHeightContent();                           // height="content"
    mObjects.measuredValue.setText("--.-");                                     // text="--.-"
    mObjects.measuredUnit.setText("°C");                                        // text="°C"
    mObjects.measuredUnit.setPadBottom(4);                                      // pad-bottom="4px"
    mObjects.temperatureDelta.setText("waiting for sensor");                    // text="waiting for sensor"
    mObjects.temperatureDelta.setTextColor(lv_color_hex(0x60717b));             // text-color="#60717B"
    mObjects.sidePanel.setFlexGrow(1);                                          // flex-grow="1"
    mObjects.sidePanel.setHeight(LV_PCT(100));                                  // height="100%"
    mObjects.sidePanel.setLayout(LV_LAYOUT_FLEX);                               // layout="flex"
    mObjects.sidePanel.setFlexFlow(LV_FLEX_FLOW_COLUMN);                        // flex-flow="column"
    mObjects.sidePanel.setPadGap(5);                                            // pad-gap="5px"
    mObjects.targetCard.setWidth(LV_PCT(100));                                  // width="100%"
    mObjects.targetCard.setHeight(68);                                          // height="68px"
    mObjects.targetCard.setLayout(LV_LAYOUT_FLEX);                              // layout="flex"
    mObjects.targetCard.setFlexFlow(LV_FLEX_FLOW_COLUMN);                       // flex-flow="column"
    mObjects.targetCard.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.targetCard.setBgColor(lv_color_hex(0x13202b));                     // bg-color="#13202B"
    mObjects.targetCard.setBgOpa(255);                                          // bg-opa="100%"
    mObjects.targetCard.setRadius(9);                                           // radius="9px"
    mObjects.targetCard.setBorderColor(lv_color_hex(0x8ab4f4));                 // border-color="#8AB4F4"
    mObjects.targetCard.setBorderWidth(1);                                      // border-width="1px"
    mObjects.targetCard.setBorderSide(LV_BORDER_SIDE_FULL);                     // border-side="full"
    mObjects.targetLabel.setText("TARGET");                                     // text="TARGET"
    mObjects.targetLabel.setTextColor(lv_color_hex(0x8ab4f4));                  // text-color="#8AB4F4"
    mObjects._namelessContainer_2.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_2.setFlexFlow(LV_FLEX_FLOW_ROW);                // flex-flow="row"
    mObjects._namelessContainer_2.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects._namelessContainer_2.setWidth(LV_PCT(100));                        // width="100%"
    mObjects._namelessContainer_2.setHeightContent();                           // height="content"
    mObjects._namelessContainer_2.setPadLeft(5);                                // pad-left="5px"
    mObjects._namelessContainer_2.setPadRight(5);                               // pad-right="5px"
    mObjects.targetRotateLeft.setWidth(12);                                     // width="12px"
    mObjects.targetRotateLeft.setTextAlign(LV_TEXT_ALIGN_CENTER);               // text-align="center"
    mObjects.targetRotateLeft.setText("");                                      // text=""
    mObjects._namelessContainer_3.setFlexGrow(1);                               // flex-grow="1"
    mObjects._namelessContainer_3.setHeightContent();                           // height="content"
    mObjects._namelessContainer_3.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_3.setFlexFlow(LV_FLEX_FLOW_ROW);                // flex-flow="row"
    mObjects._namelessContainer_3.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END); // flex-align="center end end"
    mObjects._namelessContainer_3.setPadGap(3);                                 // pad-gap="3px"
    mObjects.targetValue.setText("32.0");                                       // text="32.0"
    mObjects.targetUnit.setText("°C");                                          // text="°C"
    mObjects.targetUnit.setPadBottom(2);                                        // pad-bottom="2px"
    mObjects.targetRotateRight.setWidth(12);                                    // width="12px"
    mObjects.targetRotateRight.setTextAlign(LV_TEXT_ALIGN_CENTER);              // text-align="center"
    mObjects.targetRotateRight.setText("");                                     // text=""
    mObjects.humidity.body.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.humidity.body.setFlexFlow(LV_FLEX_FLOW_ROW);                       // flex-flow="row"
    mObjects.humidity.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.humidity.body.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.humidity.body.setFlexGrow(1);                                      // flex-grow="1"
    mObjects.humidity.body.setPadLeft(7);                                       // pad-left="7px"
    mObjects.humidity.body.setPadRight(7);                                      // pad-right="7px"
    mObjects.humidity.body.setPadTop(2);                                        // pad-top="2px"
    mObjects.humidity.body.setPadBottom(2);                                     // pad-bottom="2px"
    mObjects.humidity.body.setPadGap(7);                                        // pad-gap="7px"
    mObjects.humidity.body.setBgColor(lv_color_hex(0x111a21));                  // bg-color="#111A21"
    mObjects.humidity.body.setBgOpa(255);                                       // bg-opa="100%"
    mObjects.humidity.body.setRadius(7);                                        // radius="7px"
    mObjects.humidity.body.setBorderColor(lv_color_hex(0x26343d));              // border-color="#26343D"
    mObjects.humidity.body.setBorderWidth(1);                                   // border-width="1px"
    mObjects.humidity.body.setBorderSide(LV_BORDER_SIDE_FULL);                  // border-side="full"
    mObjects.humidity.icon.setSource(&SimpleScreen_image_humidity16);           // src="humidity16"
    mObjects.humidity.icon.setWidth(16);                                        // width="16px"
    mObjects.humidity.icon.setHeight(16);                                       // height="16px"
    mObjects.humidity.icon.setOpa(191);                                         // opa="75%"
    mObjects.humidity._namelessContainer_0.setLayout(LV_LAYOUT_FLEX);           // layout="flex"
    mObjects.humidity._namelessContainer_0.setFlexFlow(LV_FLEX_FLOW_COLUMN);    // flex-flow="column"
    mObjects.humidity._namelessContainer_0.setFlexGrow(1);                      // flex-grow="1"
    mObjects.humidity._namelessContainer_0.setHeightContent();                  // height="content"
    mObjects.humidity._namelessContainer_0.setPadGap(0);                        // pad-gap="0px"
    mObjects.humidity.label.setText("HUMIDITY");                                // text="HUMIDITY"
    mObjects.humidity.label.setTextColor(lv_color_hex(0x778791));               // text-color="#778791"
    mObjects.humidity.value.setText("--.-%");                                   // text="--.-%"
    mObjects.humidity.value.setTextColor(lv_color_hex(0xffffff));               // text-color="#FFFFFF"
    mObjects.heater.body.setLayout(LV_LAYOUT_FLEX);                             // layout="flex"
    mObjects.heater.body.setFlexFlow(LV_FLEX_FLOW_ROW);                         // flex-flow="row"
    mObjects.heater.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.heater.body.setWidth(LV_PCT(100));                                 // width="100%"
    mObjects.heater.body.setFlexGrow(1);                                        // flex-grow="1"
    mObjects.heater.body.setPadLeft(7);                                         // pad-left="7px"
    mObjects.heater.body.setPadRight(7);                                        // pad-right="7px"
    mObjects.heater.body.setPadTop(2);                                          // pad-top="2px"
    mObjects.heater.body.setPadBottom(2);                                       // pad-bottom="2px"
    mObjects.heater.body.setPadGap(7);                                          // pad-gap="7px"
    mObjects.heater.body.setBgColor(lv_color_hex(0x111a21));                    // bg-color="#111A21"
    mObjects.heater.body.setBgOpa(255);                                         // bg-opa="100%"
    mObjects.heater.body.setRadius(7);                                          // radius="7px"
    mObjects.heater.body.setBorderColor(lv_color_hex(0x26343d));                // border-color="#26343D"
    mObjects.heater.body.setBorderWidth(1);                                     // border-width="1px"
    mObjects.heater.body.setBorderSide(LV_BORDER_SIDE_FULL);                    // border-side="full"
    mObjects.heater.icon.setSource(&SimpleScreen_image_heat16);                 // src="heat16"
    mObjects.heater.icon.setWidth(16);                                          // width="16px"
    mObjects.heater.icon.setHeight(16);                                         // height="16px"
    mObjects.heater.icon.setOpa(191);                                           // opa="75%"
    mObjects.heater._namelessContainer_0.setLayout(LV_LAYOUT_FLEX);             // layout="flex"
    mObjects.heater._namelessContainer_0.setFlexFlow(LV_FLEX_FLOW_COLUMN);      // flex-flow="column"
    mObjects.heater._namelessContainer_0.setFlexGrow(1);                        // flex-grow="1"
    mObjects.heater._namelessContainer_0.setHeightContent();                    // height="content"
    mObjects.heater._namelessContainer_0.setPadGap(0);                          // pad-gap="0px"
    mObjects.heater.label.setText("HEATER");                                    // text="HEATER"
    mObjects.heater.label.setTextColor(lv_color_hex(0x778791));                 // text-color="#778791"
    mObjects.heater.value.setText("0%");                                        // text="0%"
    mObjects.heater.value.setTextColor(lv_color_hex(0xffffff));                 // text-color="#FFFFFF"
    mObjects.fan.body.setLayout(LV_LAYOUT_FLEX);                                // layout="flex"
    mObjects.fan.body.setFlexFlow(LV_FLEX_FLOW_ROW);                            // flex-flow="row"
    mObjects.fan.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.fan.body.setWidth(LV_PCT(100));                                    // width="100%"
    mObjects.fan.body.setFlexGrow(1);                                           // flex-grow="1"
    mObjects.fan.body.setPadLeft(7);                                            // pad-left="7px"
    mObjects.fan.body.setPadRight(7);                                           // pad-right="7px"
    mObjects.fan.body.setPadTop(2);                                             // pad-top="2px"
    mObjects.fan.body.setPadBottom(2);                                          // pad-bottom="2px"
    mObjects.fan.body.setPadGap(7);                                             // pad-gap="7px"
    mObjects.fan.body.setBgColor(lv_color_hex(0x111a21));                       // bg-color="#111A21"
    mObjects.fan.body.setBgOpa(255);                                            // bg-opa="100%"
    mObjects.fan.body.setRadius(7);                                             // radius="7px"
    mObjects.fan.body.setBorderColor(lv_color_hex(0x26343d));                   // border-color="#26343D"
    mObjects.fan.body.setBorderWidth(1);                                        // border-width="1px"
    mObjects.fan.body.setBorderSide(LV_BORDER_SIDE_FULL);                       // border-side="full"
    mObjects.fan.icon.setSource(&SimpleScreen_image_fan16);                     // src="fan16"
    mObjects.fan.icon.setWidth(16);                                             // width="16px"
    mObjects.fan.icon.setHeight(16);                                            // height="16px"
    mObjects.fan.icon.setOpa(191);                                              // opa="75%"
    mObjects.fan._namelessContainer_0.setLayout(LV_LAYOUT_FLEX);                // layout="flex"
    mObjects.fan._namelessContainer_0.setFlexFlow(LV_FLEX_FLOW_COLUMN);         // flex-flow="column"
    mObjects.fan._namelessContainer_0.setFlexGrow(1);                           // flex-grow="1"
    mObjects.fan._namelessContainer_0.setHeightContent();                       // height="content"
    mObjects.fan._namelessContainer_0.setPadGap(0);                             // pad-gap="0px"
    mObjects.fan.label.setText("FAN");                                          // text="FAN"
    mObjects.fan.label.setTextColor(lv_color_hex(0x778791));                    // text-color="#778791"
    mObjects.fan.value.setText("OFF");                                          // text="OFF"
    mObjects.fan.value.setTextColor(lv_color_hex(0xffffff));                    // text-color="#FFFFFF"
    mObjects.footer.setWidth(LV_PCT(100));                                      // width="100%"
    mObjects.footer.setHeight(20);                                              // height="20px"
    mObjects.pressHint.setText("PRESS: START/STOP");                            // text="PRESS: START/STOP"
    mObjects.holdHint.setText("HOLD: MENU");                                    // text="HOLD: MENU"
}

void SimpleScreenGui::show(){
    mObjects.root.setScreen();
}

SimpleScreenGui::Objects& SimpleScreenGui::objects(){
    return mObjects;
}

// ============================================================
// === Component Structs ======================================
// ============================================================

SimpleScreenGui::MetricCard::MetricCard(LvObject& parent):
    body(parent),
    icon(body),
    _namelessContainer_0(body),
    label(_namelessContainer_0),
    value(_namelessContainer_0){}

// ============================================================
// === Objects Struct =========================================
// ============================================================

SimpleScreenGui::Objects::Objects():
    root(),
    wrapper(root),
    header(wrapper),
    statusPill(header),
    statusIcon(statusPill),
    statusText(statusPill),
    content(wrapper),
    gaugePanel(content),
    arcContainer(gaugePanel),
    arcBg(arcContainer),
    arcMeasured(arcContainer),
    arcTarget(arcContainer),
    _namelessContainer_0(gaugePanel),
    measuredLabel(_namelessContainer_0),
    _namelessContainer_1(_namelessContainer_0),
    measuredValue(_namelessContainer_1),
    measuredUnit(_namelessContainer_1),
    temperatureDelta(_namelessContainer_0),
    sidePanel(content),
    targetCard(sidePanel),
    targetLabel(targetCard),
    _namelessContainer_2(targetCard),
    targetRotateLeft(_namelessContainer_2),
    _namelessContainer_3(_namelessContainer_2),
    targetValue(_namelessContainer_3),
    targetUnit(_namelessContainer_3),
    targetRotateRight(_namelessContainer_2),
    humidity(sidePanel),
    heater(sidePanel),
    fan(sidePanel),
    footer(root),
    pressHint(footer),
    holdHint(footer){}

} // namespace

