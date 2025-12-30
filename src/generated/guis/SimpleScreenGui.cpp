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
    mObjects.root.setBgColor(lv_color_hex(0x000000));                           // bg-color="#000000"
    mObjects.root.setTextColor(lv_color_hex(0xffffff));                         // text-color="#ffffff"
    mObjects.wrapper.setLayout(LV_LAYOUT_FLEX);                                 // layout="flex"
    mObjects.wrapper.setFlexFlow(LV_FLEX_FLOW_ROW);                             // flex-flow="row"
    mObjects.wrapper.setWidth(LV_PCT(100));                                     // width="100%"
    mObjects.wrapper.setHeight(LV_PCT(100));                                    // height="100%"
    mObjects._namelessContainer_0.setWidth(240);                                // width="240px"
    mObjects._namelessContainer_0.setHeight(240);                               // height="240px"
    mObjects.arcContainer.setAlign(LV_ALIGN_CENTER, 0, 0);                      // align="center"
    mObjects.arcContainer.setWidth(224);                                        // width="224px"
    mObjects.arcContainer.setHeight(224);                                       // height="224px"
    mObjects.arcBg.setAlign(LV_ALIGN_CENTER, 0, 0);                             // align="center"
    mObjects.arcBg.setWidth(212);                                               // width="212px"
    mObjects.arcBg.setHeight(212);                                              // height="212px"
    mObjects.arcBg.setArcBgWidth(20);                                           // arc-bg-width="20px"
    mObjects.arcBg.setArcBgAngles(0, 359);                                      // arc-bg-angles="0 359"
    mObjects.arcBg.setArcBgColor(lv_color_hex(0x666666));                       // arc-bg-color="#666"
    mObjects.arcBg.setArcOpa(0);                                                // arc-opa="0%"
    mObjects.arcBg.setArcBgOpa(255);                                            // arc-bg-opa="100%"
    mObjects.arcBg.setArcBgRounded(false);                                      // arc-bg-rounded="0"
    mObjects.arcMeasured.setAlign(LV_ALIGN_CENTER, 0, 0);                       // align="center"
    mObjects.arcMeasured.setWidth(224);                                         // width="224px"
    mObjects.arcMeasured.setHeight(224);                                        // height="224px"
    mObjects.arcMeasured.setArcWidth(20);                                       // arc-width="20px"
    mObjects.arcMeasured.setArcAngles(0, 60);                                   // arc-angles="0 60"
    mObjects.arcMeasured.setArcColor(lv_color_hex(0x8ab4f4));                   // arc-color="#8AB4F4"
    mObjects.arcMeasured.setArcOpa(255);                                        // arc-opa="100%"
    mObjects.arcMeasured.setArcBgOpa(0);                                        // arc-bg-opa="0%"
    mObjects.arcMeasured.setArcRounded(false);                                  // arc-rounded="0"
    mObjects.arcMeasuredEnd.setAlign(LV_ALIGN_CENTER, 0, 0);                    // align="center"
    mObjects.arcMeasuredEnd.setWidth(224);                                      // width="224px"
    mObjects.arcMeasuredEnd.setHeight(224);                                     // height="224px"
    mObjects.arcMeasuredEnd.setArcWidth(20);                                    // arc-width="20px"
    mObjects.arcMeasuredEnd.setArcAngles(60, 61);                               // arc-angles="60 61"
    mObjects.arcMeasuredEnd.setArcColor(lv_color_hex(0x000000));                // arc-color="#000"
    mObjects.arcMeasuredEnd.setArcOpa(255);                                     // arc-opa="100%"
    mObjects.arcMeasuredEnd.setArcBgOpa(0);                                     // arc-bg-opa="0%"
    mObjects.arcMeasuredEnd.setArcRounded(false);                               // arc-rounded="0"
    mObjects.arcMeasuredBorder.setAlign(LV_ALIGN_CENTER, 0, 0);                 // align="center"
    mObjects.arcMeasuredBorder.setWidth(186);                                   // width="186px"
    mObjects.arcMeasuredBorder.setHeight(186);                                  // height="186px"
    mObjects.arcMeasuredBorder.setArcWidth(2);                                  // arc-width="2px"
    mObjects.arcMeasuredBorder.setArcAngles(0, 60);                             // arc-angles="0 60"
    mObjects.arcMeasuredBorder.setArcColor(lv_color_hex(0x000000));             // arc-color="#000"
    mObjects.arcMeasuredBorder.setArcOpa(255);                                  // arc-opa="100%"
    mObjects.arcMeasuredBorder.setArcBgOpa(0);                                  // arc-bg-opa="0%"
    mObjects.arcMeasuredBorder.setArcRounded(false);                            // arc-rounded="0"
    mObjects.arcTarget.setAlign(LV_ALIGN_CENTER, 0, 0);                         // align="center"
    mObjects.arcTarget.setWidth(200);                                           // width="200px"
    mObjects.arcTarget.setHeight(200);                                          // height="200px"
    mObjects.arcTarget.setArcWidth(20);                                         // arc-width="20px"
    mObjects.arcTarget.setArcAngles(0, 120);                                    // arc-angles="0 120"
    mObjects.arcTarget.setArcColor(lv_color_hex(0xffffff));                     // arc-color="#FFF"
    mObjects.arcTarget.setArcOpa(255);                                          // arc-opa="100%"
    mObjects.arcTarget.setArcBgOpa(0);                                          // arc-bg-opa="0%"
    mObjects.arcTarget.setArcRounded(false);                                    // arc-rounded="0"
    mObjects.arcTargetEnd.setAlign(LV_ALIGN_CENTER, 0, 0);                      // align="center"
    mObjects.arcTargetEnd.setWidth(200);                                        // width="200px"
    mObjects.arcTargetEnd.setHeight(200);                                       // height="200px"
    mObjects.arcTargetEnd.setArcWidth(20);                                      // arc-width="20px"
    mObjects.arcTargetEnd.setArcAngles(120, 121);                               // arc-angles="120 121"
    mObjects.arcTargetEnd.setArcColor(lv_color_hex(0x000000));                  // arc-color="#000"
    mObjects.arcTargetEnd.setArcOpa(255);                                       // arc-opa="100%"
    mObjects.arcTargetEnd.setArcBgOpa(0);                                       // arc-bg-opa="0%"
    mObjects.arcTargetEnd.setArcRounded(false);                                 // arc-rounded="0"
    mObjects.arcTargetBorder.setAlign(LV_ALIGN_CENTER, 0, 0);                   // align="center"
    mObjects.arcTargetBorder.setWidth(202);                                     // width="202px"
    mObjects.arcTargetBorder.setHeight(202);                                    // height="202px"
    mObjects.arcTargetBorder.setArcWidth(2);                                    // arc-width="2px"
    mObjects.arcTargetBorder.setArcAngles(0, 120);                              // arc-angles="0 120"
    mObjects.arcTargetBorder.setArcColor(lv_color_hex(0x000000));               // arc-color="#000"
    mObjects.arcTargetBorder.setArcOpa(255);                                    // arc-opa="100%"
    mObjects.arcTargetBorder.setArcBgOpa(0);                                    // arc-bg-opa="0%"
    mObjects.arcTargetBorder.setArcRounded(false);                              // arc-rounded="0"
    mObjects._namelessContainer_1.setAlign(LV_ALIGN_CENTER, 0, 0);              // align="center"
    mObjects._namelessContainer_1.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_1.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_1.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.tempLabel.setText("TEMPERATURE");                                  // text="TEMPERATURE"
    mObjects.tempValue.setText("25.0");                                         // text="25.0"
    mObjects.tempValue.setMargin(0, 0, 4, 4);                                   // margin="0 0 4px 4px"
    mObjects.tempUnit.setText("°C");                                            // text="°C"
    mObjects.infoTiles.setFlexGrow(1);                                          // flex-grow="1"
    mObjects.infoTiles.setHeight(LV_PCT(100));                                  // height="100%"
    mObjects.infoTiles.setPad(0, 8, 8, 8);                                      // pad="0 8px 8px 8px"
    mObjects.infoTiles.setPadGap(8);                                            // pad-gap="8px"
    mObjects.infoTiles.setLayout(LV_LAYOUT_FLEX);                               // layout="flex"
    mObjects.infoTiles.setFlexFlow(LV_FLEX_FLOW_COLUMN);                        // flex-flow="column"
    mObjects.infoTiles.setFlexAlign(LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START); // flex-align="space-around start start"
    mObjects.onoffInfo.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.onoffInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                   // flex-flow="column"
    mObjects.onoffInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.onoffInfo.body.setBgColor(lv_color_hex(0x00aa00));                 // bg-color="#0A0"
    mObjects.onoffInfo.body.setBgOpa(128);                                      // bg-opa="50%"
    mObjects.onoffInfo.body.setRadius(8);                                       // radius="8px"
    mObjects.onoffInfo.body.setTextColor(lv_color_hex(0xffffff));               // text-color="#fff"
    mObjects.onoffInfo.body.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.onoffInfo.body.setFlexGrow(1);                                     // flex-grow="1"
    mObjects.onoffInfo.icon.setSource(&SimpleScreen_image_power16);             // src="power16"
    mObjects.onoffInfo.icon.setWidth(16);                                       // width="16"
    mObjects.onoffInfo.icon.setHeight(16);                                      // height="16"
    mObjects.onoffInfo.value.setText("ON");                                     // text="ON"
    mObjects.onoffInfo.value.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.onoffInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                // text-align="center"
    mObjects.tempInfo.body.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.tempInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                    // flex-flow="column"
    mObjects.tempInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.tempInfo.body.setBgColor(lv_color_hex(0x0088cc));                  // bg-color="#08C"
    mObjects.tempInfo.body.setBgOpa(128);                                       // bg-opa="50%"
    mObjects.tempInfo.body.setRadius(8);                                        // radius="8px"
    mObjects.tempInfo.body.setTextColor(lv_color_hex(0xffffff));                // text-color="#fff"
    mObjects.tempInfo.body.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.tempInfo.body.setFlexGrow(1);                                      // flex-grow="1"
    mObjects.tempInfo.icon.setSource(&SimpleScreen_image_temperature16);        // src="temperature16"
    mObjects.tempInfo.icon.setWidth(16);                                        // width="16"
    mObjects.tempInfo.icon.setHeight(16);                                       // height="16"
    mObjects.tempInfo.value.setText("20.0°C");                                  // text="20.0°C"
    mObjects.tempInfo.value.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.tempInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                 // text-align="center"
    mObjects.humidInfo.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.humidInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                   // flex-flow="column"
    mObjects.humidInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.humidInfo.body.setBgColor(lv_color_hex(0x666666));                 // bg-color="#666"
    mObjects.humidInfo.body.setBgOpa(128);                                      // bg-opa="50%"
    mObjects.humidInfo.body.setRadius(8);                                       // radius="8px"
    mObjects.humidInfo.body.setTextColor(lv_color_hex(0xffffff));               // text-color="#fff"
    mObjects.humidInfo.body.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.humidInfo.body.setFlexGrow(1);                                     // flex-grow="1"
    mObjects.humidInfo.icon.setSource(&SimpleScreen_image_humidity16);          // src="humidity16"
    mObjects.humidInfo.icon.setWidth(16);                                       // width="16"
    mObjects.humidInfo.icon.setHeight(16);                                      // height="16"
    mObjects.humidInfo.value.setText("50.0%");                                  // text="50.0%"
    mObjects.humidInfo.value.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.humidInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                // text-align="center"
    mObjects.heatInfo.body.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.heatInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                    // flex-flow="column"
    mObjects.heatInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.heatInfo.body.setBgColor(lv_color_hex(0x666666));                  // bg-color="#666"
    mObjects.heatInfo.body.setBgOpa(128);                                       // bg-opa="50%"
    mObjects.heatInfo.body.setRadius(8);                                        // radius="8px"
    mObjects.heatInfo.body.setTextColor(lv_color_hex(0xffffff));                // text-color="#fff"
    mObjects.heatInfo.body.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.heatInfo.body.setFlexGrow(1);                                      // flex-grow="1"
    mObjects.heatInfo.icon.setSource(&SimpleScreen_image_heat16);               // src="heat16"
    mObjects.heatInfo.icon.setWidth(16);                                        // width="16"
    mObjects.heatInfo.icon.setHeight(16);                                       // height="16"
    mObjects.heatInfo.value.setText("50%");                                     // text="50%"
    mObjects.heatInfo.value.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.heatInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                 // text-align="center"
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

SimpleScreenGui::InfoTile::InfoTile(LvObject& parent):
    body(parent),
    icon(body),
    value(body){}

// ============================================================
// === Objects Struct =========================================
// ============================================================

SimpleScreenGui::Objects::Objects():
    root(),
    wrapper(root),
    _namelessContainer_0(wrapper),
    arcContainer(_namelessContainer_0),
    arcBg(arcContainer),
    arcMeasured(arcContainer),
    arcMeasuredEnd(arcContainer),
    arcMeasuredBorder(arcContainer),
    arcTarget(arcContainer),
    arcTargetEnd(arcContainer),
    arcTargetBorder(arcContainer),
    _namelessContainer_1(_namelessContainer_0),
    tempLabel(_namelessContainer_1),
    tempValue(_namelessContainer_1),
    tempUnit(_namelessContainer_1),
    infoTiles(wrapper),
    onoffInfo(infoTiles),
    tempInfo(infoTiles),
    humidInfo(infoTiles),
    heatInfo(infoTiles){}

} // namespace

