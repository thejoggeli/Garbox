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
    mObjects.wrapper.setLayout(LV_LAYOUT_FLEX);                                 // layout="flex"
    mObjects.wrapper.setFlexFlow(LV_FLEX_FLOW_ROW);                             // flex-flow="row"
    mObjects.wrapper.setWidth(LV_PCT(100));                                     // width="100%"
    mObjects.wrapper.setHeight(LV_PCT(100));                                    // height="100%"
    mObjects._namelessContainer_0.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_0.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_0.setWidth(208);                                // width="208px"
    mObjects._namelessContainer_0.setHeight(LV_PCT(100));                       // height="100%"
    mObjects.tempGraph.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.tempGraph.body.setFlexFlow(LV_FLEX_FLOW_ROW);                      // flex-flow="row"
    mObjects.tempGraph.body.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.tempGraph.body.setHeight(126);                                     // height="126px"
    mObjects.tempGraph.body.setPadTop(8);                                       // pad-top="8px"
    mObjects.tempGraph.body.setPadBottom(4);                                    // pad-bottom="4px"
    mObjects.tempGraph.labelContainer.setWidth(22);                             // width="22px"
    mObjects.tempGraph.labelContainer.setHeight(LV_PCT(100));                   // height="100%"
    mObjects.tempGraph.labelContainer.setLayout(LV_LAYOUT_FLEX);                // layout="flex"
    mObjects.tempGraph.labelContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);         // flex-flow="column"
    mObjects.tempGraph.labelContainer.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.tempGraph._namelessContainer_0.setFlexGrow(1);                     // flex-grow="1"
    mObjects.tempGraph._namelessContainer_0.setHeight(LV_PCT(100));             // height="100%"
    mObjects.tempGraph._namelessContainer_0.setBorderColor(lv_color_hex(0xffffff)); // border-color="#fff"
    mObjects.tempGraph._namelessContainer_0.setBorderWidth(1);                  // border-width="1px"
    mObjects.tempGraph._namelessContainer_0.setBorderSide(LV_BORDER_SIDE_FULL); // border-side="full"
    mObjects.tempGraph._namelessContainer_0.setRadius(8);                       // radius="8px"
    mObjects.tempGraph.chart.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.tempGraph.chart.setHeight(LV_PCT(100));                            // height="100%"
    mObjects.tempGraph.value.setText("");                                       // text=""
    mObjects.tempGraph.value.setWidthContent();                                 // width="content"
    mObjects.tempGraph.value.setHeightContent();                                // height="content"
    mObjects.tempGraph.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);                 // text-align="right"
    mObjects.tempGraph.value.setAlign(LV_ALIGN_TOP_RIGHT, -4, 4);               // align="top-right -4px 4px"
    mObjects.tempGraph.value.setBgColor(lv_color_hex(0x000000));                // bg-color="#000"
    mObjects.tempGraph.value.setBgOpa(102);                                     // bg-opa="40%"
    mObjects.tempGraph.value.setRadius(4);                                      // radius="4px"
    mObjects.powerGraph.body.setLayout(LV_LAYOUT_FLEX);                         // layout="flex"
    mObjects.powerGraph.body.setFlexFlow(LV_FLEX_FLOW_ROW);                     // flex-flow="row"
    mObjects.powerGraph.body.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.powerGraph.body.setHeight(96);                                     // height="96px"
    mObjects.powerGraph.body.setPadTop(4);                                      // pad-top="4px"
    mObjects.powerGraph.labelContainer.setWidth(22);                            // width="22px"
    mObjects.powerGraph.labelContainer.setHeight(LV_PCT(100));                  // height="100%"
    mObjects.powerGraph.labelContainer.setLayout(LV_LAYOUT_FLEX);               // layout="flex"
    mObjects.powerGraph.labelContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);        // flex-flow="column"
    mObjects.powerGraph.labelContainer.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.powerGraph._namelessContainer_0.setFlexGrow(1);                    // flex-grow="1"
    mObjects.powerGraph._namelessContainer_0.setHeight(LV_PCT(100));            // height="100%"
    mObjects.powerGraph._namelessContainer_0.setBorderColor(lv_color_hex(0xffffff)); // border-color="#fff"
    mObjects.powerGraph._namelessContainer_0.setBorderWidth(1);                 // border-width="1px"
    mObjects.powerGraph._namelessContainer_0.setBorderSide(LV_BORDER_SIDE_FULL); // border-side="full"
    mObjects.powerGraph._namelessContainer_0.setRadius(8);                      // radius="8px"
    mObjects.powerGraph.chart.setWidth(LV_PCT(100));                            // width="100%"
    mObjects.powerGraph.chart.setHeight(LV_PCT(100));                           // height="100%"
    mObjects.powerGraph.value.setText("");                                      // text=""
    mObjects.powerGraph.value.setWidthContent();                                // width="content"
    mObjects.powerGraph.value.setHeightContent();                               // height="content"
    mObjects.powerGraph.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);                // text-align="right"
    mObjects.powerGraph.value.setAlign(LV_ALIGN_TOP_RIGHT, -4, 4);              // align="top-right -4px 4px"
    mObjects.powerGraph.value.setBgColor(lv_color_hex(0x000000));               // bg-color="#000"
    mObjects.powerGraph.value.setBgOpa(102);                                    // bg-opa="40%"
    mObjects.powerGraph.value.setRadius(4);                                     // radius="4px"
    mObjects.timeAxis.setWidth(LV_PCT(100));                                    // width="100%"
    mObjects.timeAxis.setFlexGrow(1);                                           // flex-grow="1"
    mObjects.timeAxis.setLayout(LV_LAYOUT_FLEX);                                // layout="flex"
    mObjects.timeAxis.setFlexFlow(LV_FLEX_FLOW_ROW);                            // flex-flow="row"
    mObjects.timeAxis.setFlexAlign(LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="space-around center center"
    mObjects.timeAxis.setPadLeft(22);                                           // pad-left="22px"
    mObjects.timeAxis.setPadBottom(1);                                          // pad-bottom="1px"
    mObjects.t4.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t4.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t4.setTextColor(lv_color_hex(0x808080));                           // text-color="#808080"
    mObjects.t4.setText("12h");                                                 // text="12h"
    mObjects.t4.setPadLeft(1);                                                  // pad-left="1px"
    mObjects.t3.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t3.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t3.setTextColor(lv_color_hex(0x808080));                           // text-color="#808080"
    mObjects.t3.setText("9h");                                                  // text="9h"
    mObjects.t2.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t2.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t2.setTextColor(lv_color_hex(0x808080));                           // text-color="#808080"
    mObjects.t2.setText("6h");                                                  // text="6h"
    mObjects.t1.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t1.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t1.setTextColor(lv_color_hex(0x808080));                           // text-color="#808080"
    mObjects.t1.setText("3h");                                                  // text="3h"
    mObjects.t0.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t0.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t0.setTextColor(lv_color_hex(0x808080));                           // text-color="#808080"
    mObjects.t0.setText("Now");                                                 // text="Now"
    mObjects.t0.setPadRight(1);                                                 // pad-right="1px"
    mObjects._namelessContainer_1.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_1.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_1.setFlexGrow(1);                               // flex-grow="1"
    mObjects._namelessContainer_1.setHeight(LV_PCT(100));                       // height="100%"
    mObjects._namelessContainer_1.setPad(8, 8, 8, 0);                           // pad="8px 8px 8px 0px"
    mObjects.systemStateBg.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.systemStateBg.setHeight(22);                                       // height="22px"
    mObjects.systemStateBg.setBgColor(lv_color_hex(0x70bc47));                  // bg-color="#70bc47"
    mObjects.systemStateBg.setBgOpa(255);                                       // bg-opa="100%"
    mObjects.systemStateBg.setRadius(8);                                        // radius="8px"
    mObjects.systemState.setAlign(LV_ALIGN_CENTER, 0, 0);                       // align="center"
    mObjects.systemState.setText("Regulating");                                 // text="Regulating"
    mObjects.systemState.setTextColor(lv_color_hex(0x000000));                  // text-color="#000"
    mObjects.menuContainer.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.menuContainer.setFlexGrow(1);                                      // flex-grow="1"
    mObjects.menuContainer.setPadTop(16);                                       // pad-top="16px"
    mObjects.menuContainer.setPadBottom(8);                                     // pad-bottom="8px"
    mObjects.menuContainer.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.menuContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);                    // flex-flow="column"
    mObjects.menuContainer.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.menuArrUp.setSource(&MainScreen_image_arrow_up);                   // src="arrow-up"
    mObjects.menuArrUp.setOpa(102);                                             // opa="40%"
    mObjects._namelessContainer_2.setWidth(LV_PCT(100));                        // width="100%"
    mObjects._namelessContainer_2.setHeight(4);                                 // height="4px"
    mObjects._namelessContainer_3.setWidth(LV_PCT(100));                        // width="100%"
    mObjects._namelessContainer_3.setFlexGrow(1);                               // flex-grow="1"
    mObjects._namelessContainer_3.setBorderColor(lv_color_hex(0xffffff));       // border-color="#fff"
    mObjects._namelessContainer_3.setBorderWidth(1);                            // border-width="1px"
    mObjects._namelessContainer_3.setBorderSide(LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_BOTTOM); // border-side="top bottom"
    mObjects._namelessContainer_3.setLayout(LV_LAYOUT_FLEX);                    // layout="flex"
    mObjects._namelessContainer_3.setFlexFlow(LV_FLEX_FLOW_COLUMN);             // flex-flow="column"
    mObjects._namelessContainer_3.setFlexAlign(LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="space-evenly center center"
    mObjects._namelessMenuRow_0.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessMenuRow_0.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessMenuRow_0.body.setWidth(LV_PCT(100));                     // width="100%"
    mObjects._namelessMenuRow_0.body.setHeightContent();                        // height="content"
    mObjects._namelessMenuRow_0.label.setText("Power");                         // text="Power"
    mObjects._namelessMenuRow_0.label.setTextAlign(LV_TEXT_ALIGN_LEFT);         // text-align="left"
    mObjects._namelessMenuRow_0.label.setFlexGrow(1);                           // flex-grow="1"
    mObjects._namelessMenuRow_0.label.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_0.value.setText("On");                            // text="On"
    mObjects._namelessMenuRow_0.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);        // text-align="right"
    mObjects._namelessMenuRow_0.value.setWidthContent();                        // width="content"
    mObjects._namelessMenuRow_0.value.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_1.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessMenuRow_1.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessMenuRow_1.body.setWidth(LV_PCT(100));                     // width="100%"
    mObjects._namelessMenuRow_1.body.setHeightContent();                        // height="content"
    mObjects._namelessMenuRow_1.label.setText("Target");                        // text="Target"
    mObjects._namelessMenuRow_1.label.setTextAlign(LV_TEXT_ALIGN_LEFT);         // text-align="left"
    mObjects._namelessMenuRow_1.label.setFlexGrow(1);                           // flex-grow="1"
    mObjects._namelessMenuRow_1.label.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_1.value.setText("32.5°C");                        // text="32.5°C"
    mObjects._namelessMenuRow_1.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);        // text-align="right"
    mObjects._namelessMenuRow_1.value.setWidthContent();                        // width="content"
    mObjects._namelessMenuRow_1.value.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_2.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessMenuRow_2.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessMenuRow_2.body.setWidth(LV_PCT(100));                     // width="100%"
    mObjects._namelessMenuRow_2.body.setHeightContent();                        // height="content"
    mObjects._namelessMenuRow_2.label.setText("History");                       // text="History"
    mObjects._namelessMenuRow_2.label.setTextAlign(LV_TEXT_ALIGN_LEFT);         // text-align="left"
    mObjects._namelessMenuRow_2.label.setFlexGrow(1);                           // flex-grow="1"
    mObjects._namelessMenuRow_2.label.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_2.value.setText("12h");                           // text="12h"
    mObjects._namelessMenuRow_2.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);        // text-align="right"
    mObjects._namelessMenuRow_2.value.setWidthContent();                        // width="content"
    mObjects._namelessMenuRow_2.value.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_3.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessMenuRow_3.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessMenuRow_3.body.setWidth(LV_PCT(100));                     // width="100%"
    mObjects._namelessMenuRow_3.body.setHeightContent();                        // height="content"
    mObjects._namelessMenuRow_3.label.setText("Fan");                           // text="Fan"
    mObjects._namelessMenuRow_3.label.setTextAlign(LV_TEXT_ALIGN_LEFT);         // text-align="left"
    mObjects._namelessMenuRow_3.label.setFlexGrow(1);                           // flex-grow="1"
    mObjects._namelessMenuRow_3.label.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_3.value.setText("Auto");                          // text="Auto"
    mObjects._namelessMenuRow_3.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);        // text-align="right"
    mObjects._namelessMenuRow_3.value.setWidthContent();                        // width="content"
    mObjects._namelessMenuRow_3.value.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_4.body.setLayout(LV_LAYOUT_FLEX);                 // layout="flex"
    mObjects._namelessMenuRow_4.body.setFlexFlow(LV_FLEX_FLOW_ROW);             // flex-flow="row"
    mObjects._namelessMenuRow_4.body.setWidth(LV_PCT(100));                     // width="100%"
    mObjects._namelessMenuRow_4.body.setHeightContent();                        // height="content"
    mObjects._namelessMenuRow_4.label.setText("Calib");                         // text="Calib"
    mObjects._namelessMenuRow_4.label.setTextAlign(LV_TEXT_ALIGN_LEFT);         // text-align="left"
    mObjects._namelessMenuRow_4.label.setFlexGrow(1);                           // flex-grow="1"
    mObjects._namelessMenuRow_4.label.setHeightContent();                       // height="content"
    mObjects._namelessMenuRow_4.value.setText("-->");                           // text="-->"
    mObjects._namelessMenuRow_4.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);        // text-align="right"
    mObjects._namelessMenuRow_4.value.setWidthContent();                        // width="content"
    mObjects._namelessMenuRow_4.value.setHeightContent();                       // height="content"
    mObjects._namelessContainer_4.setWidth(LV_PCT(100));                        // width="100%"
    mObjects._namelessContainer_4.setHeight(4);                                 // height="4px"
    mObjects.menuArrDown.setSource(&MainScreen_image_arrow_down);               // src="arrow-down"
    mObjects.infoTiles.setWidth(LV_PCT(100));                                   // width="100%"
    mObjects.infoTiles.setHeightContent();                                      // height="content"
    mObjects.infoTiles.setLayout(LV_LAYOUT_FLEX);                               // layout="flex"
    mObjects.infoTiles.setFlexFlow(LV_FLEX_FLOW_ROW_WRAP);                      // flex-flow="row-wrap"
    mObjects.infoTiles.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_SPACE_AROUND); // flex-align="center space-around space-around"
    mObjects.infoTiles.setPadTop(4);                                            // pad-top="4px"
    mObjects.infoTiles.setPadBottom(4);                                         // pad-bottom="4px"
    mObjects.fanInfo.body.setLayout(LV_LAYOUT_FLEX);                            // layout="flex"
    mObjects.fanInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                     // flex-flow="column"
    mObjects.fanInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.fanInfo.body.setMargin(0, 0, 4, 4);                                // margin="0 0 4px 4px"
    mObjects.fanInfo.body.setOpa(191);                                          // opa="75%"
    mObjects.fanInfo.body.setWidth(LV_PCT(50));                                 // width="50%"
    mObjects.fanInfo.body.setHeightContent();                                   // height="content"
    mObjects.fanInfo.icon.setSource(&MainScreen_image_fan16);                   // src="fan16"
    mObjects.fanInfo.icon.setWidth(16);                                         // width="16"
    mObjects.fanInfo.icon.setHeight(16);                                        // height="16"
    mObjects.fanInfo.value.setText("0%");                                       // text="0%"
    mObjects.fanInfo.value.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.fanInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                  // text-align="center"
    mObjects.humidInfo.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.humidInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                   // flex-flow="column"
    mObjects.humidInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.humidInfo.body.setMargin(0, 0, 4, 4);                              // margin="0 0 4px 4px"
    mObjects.humidInfo.body.setOpa(191);                                        // opa="75%"
    mObjects.humidInfo.body.setWidth(LV_PCT(50));                               // width="50%"
    mObjects.humidInfo.body.setHeightContent();                                 // height="content"
    mObjects.humidInfo.icon.setSource(&MainScreen_image_humidity16);            // src="humidity16"
    mObjects.humidInfo.icon.setWidth(16);                                       // width="16"
    mObjects.humidInfo.icon.setHeight(16);                                      // height="16"
    mObjects.humidInfo.value.setText("0.0%");                                   // text="0.0%"
    mObjects.humidInfo.value.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.humidInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                // text-align="center"
    mObjects.powerInfo.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.powerInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                   // flex-flow="column"
    mObjects.powerInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.powerInfo.body.setMargin(0, 0, 4, 4);                              // margin="0 0 4px 4px"
    mObjects.powerInfo.body.setOpa(191);                                        // opa="75%"
    mObjects.powerInfo.body.setWidth(LV_PCT(50));                               // width="50%"
    mObjects.powerInfo.body.setHeightContent();                                 // height="content"
    mObjects.powerInfo.icon.setSource(&MainScreen_image_power16);               // src="power16"
    mObjects.powerInfo.icon.setWidth(16);                                       // width="16"
    mObjects.powerInfo.icon.setHeight(16);                                      // height="16"
    mObjects.powerInfo.value.setText("0.0W");                                   // text="0.0W"
    mObjects.powerInfo.value.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.powerInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                // text-align="center"
    mObjects.timeInfo.body.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.timeInfo.body.setFlexFlow(LV_FLEX_FLOW_COLUMN);                    // flex-flow="column"
    mObjects.timeInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.timeInfo.body.setMargin(0, 0, 4, 4);                               // margin="0 0 4px 4px"
    mObjects.timeInfo.body.setOpa(191);                                         // opa="75%"
    mObjects.timeInfo.body.setWidth(LV_PCT(50));                                // width="50%"
    mObjects.timeInfo.body.setHeightContent();                                  // height="content"
    mObjects.timeInfo.icon.setSource(&MainScreen_image_heat16);                 // src="heat16"
    mObjects.timeInfo.icon.setWidth(16);                                        // width="16"
    mObjects.timeInfo.icon.setHeight(16);                                       // height="16"
    mObjects.timeInfo.value.setText("1d 5h");                                   // text="1d 5h"
    mObjects.timeInfo.value.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.timeInfo.value.setTextAlign(LV_TEXT_ALIGN_CENTER);                 // text-align="center"
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

MainScreenGui::GraphComponent::GraphComponent(LvObject& parent):
    body(parent),
    labelContainer(body),
    _namelessContainer_0(body),
    chart(_namelessContainer_0),
    value(_namelessContainer_0){}

MainScreenGui::MenuRow::MenuRow(LvObject& parent):
    body(parent),
    label(body),
    value(body){}

MainScreenGui::InfoTile::InfoTile(LvObject& parent):
    body(parent),
    icon(body),
    value(body){}

// ============================================================
// === Objects Struct =========================================
// ============================================================

MainScreenGui::Objects::Objects():
    root(),
    wrapper(root),
    _namelessContainer_0(wrapper),
    tempGraph(_namelessContainer_0),
    powerGraph(_namelessContainer_0),
    timeAxis(_namelessContainer_0),
    t4(timeAxis),
    t3(timeAxis),
    t2(timeAxis),
    t1(timeAxis),
    t0(timeAxis),
    _namelessContainer_1(wrapper),
    systemStateBg(_namelessContainer_1),
    systemState(systemStateBg),
    menuContainer(_namelessContainer_1),
    menuArrUp(menuContainer),
    _namelessContainer_2(menuContainer),
    _namelessContainer_3(menuContainer),
    _namelessMenuRow_0(_namelessContainer_3),
    _namelessMenuRow_1(_namelessContainer_3),
    _namelessMenuRow_2(_namelessContainer_3),
    _namelessMenuRow_3(_namelessContainer_3),
    _namelessMenuRow_4(_namelessContainer_3),
    _namelessContainer_4(menuContainer),
    menuArrDown(menuContainer),
    infoTiles(_namelessContainer_1),
    fanInfo(infoTiles),
    humidInfo(infoTiles),
    powerInfo(infoTiles),
    timeInfo(infoTiles){}

} // namespace

