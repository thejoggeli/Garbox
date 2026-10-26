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
    mObjects.root.setBgColor(lv_color_hex(0x070b0e));                           // bg-color="#070B0E"
    mObjects.root.setTextColor(lv_color_hex(0xffffff));                         // text-color="#FFFFFF"
    mObjects.wrapper.setLayout(LV_LAYOUT_FLEX);                                 // layout="flex"
    mObjects.wrapper.setFlexFlow(LV_FLEX_FLOW_ROW);                             // flex-flow="row"
    mObjects.wrapper.setWidth(LV_PCT(100));                                     // width="100%"
    mObjects.wrapper.setHeight(220);                                            // height="220px"
    mObjects.wrapper.setPadLeft(6);                                             // pad-left="6px"
    mObjects.wrapper.setPadRight(6);                                            // pad-right="6px"
    mObjects.wrapper.setPadTop(5);                                              // pad-top="5px"
    mObjects.wrapper.setPadBottom(4);                                           // pad-bottom="4px"
    mObjects.wrapper.setPadGap(6);                                              // pad-gap="6px"
    mObjects.chartsPanel.setLayout(LV_LAYOUT_FLEX);                             // layout="flex"
    mObjects.chartsPanel.setFlexFlow(LV_FLEX_FLOW_COLUMN);                      // flex-flow="column"
    mObjects.chartsPanel.setWidth(194);                                         // width="194px"
    mObjects.chartsPanel.setHeight(LV_PCT(100));                                // height="100%"
    mObjects.chartsPanel.setPadGap(4);                                          // pad-gap="4px"
    mObjects.chartsHeader.setWidth(LV_PCT(100));                                // width="100%"
    mObjects.chartsHeader.setHeight(20);                                        // height="20px"
    mObjects.chartsHeader.setLayout(LV_LAYOUT_FLEX);                            // layout="flex"
    mObjects.chartsHeader.setFlexFlow(LV_FLEX_FLOW_ROW);                        // flex-flow="row"
    mObjects.chartsHeader.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.trendsTitle.setText("TRENDS");                                     // text="TRENDS"
    mObjects.trendsTitle.setFlexGrow(1);                                        // flex-grow="1"
    mObjects.trendsTitle.setTextColor(lv_color_hex(0xaab8c0));                  // text-color="#AAB8C0"
    mObjects.windowLabel.setText("1 MIN");                                      // text="1 MIN"
    mObjects.windowLabel.setTextColor(lv_color_hex(0x8ab4f4));                  // text-color="#8AB4F4"
    mObjects.tempGraph.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.tempGraph.body.setFlexFlow(LV_FLEX_FLOW_ROW);                      // flex-flow="row"
    mObjects.tempGraph.body.setWidth(LV_PCT(100));                              // width="100%"
    mObjects.tempGraph.body.setHeight(98);                                      // height="98px"
    mObjects.tempGraph.labelContainer.setWidth(18);                             // width="18px"
    mObjects.tempGraph.labelContainer.setHeight(LV_PCT(100));                   // height="100%"
    mObjects.tempGraph.labelContainer.setLayout(LV_LAYOUT_FLEX);                // layout="flex"
    mObjects.tempGraph.labelContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);         // flex-flow="column"
    mObjects.tempGraph.labelContainer.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.tempGraph._namelessContainer_0.setFlexGrow(1);                     // flex-grow="1"
    mObjects.tempGraph._namelessContainer_0.setHeight(LV_PCT(100));             // height="100%"
    mObjects.tempGraph._namelessContainer_0.setBgColor(lv_color_hex(0x0d141a)); // bg-color="#0D141A"
    mObjects.tempGraph._namelessContainer_0.setBgOpa(255);                      // bg-opa="100%"
    mObjects.tempGraph._namelessContainer_0.setBorderColor(lv_color_hex(0x26343d)); // border-color="#26343D"
    mObjects.tempGraph._namelessContainer_0.setBorderWidth(1);                  // border-width="1px"
    mObjects.tempGraph._namelessContainer_0.setBorderSide(LV_BORDER_SIDE_FULL); // border-side="full"
    mObjects.tempGraph._namelessContainer_0.setRadius(7);                       // radius="7px"
    mObjects.tempGraph.chart.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.tempGraph.chart.setHeight(LV_PCT(100));                            // height="100%"
    mObjects.tempGraph.value.setText("");                                       // text=""
    mObjects.tempGraph.value.setWidthContent();                                 // width="content"
    mObjects.tempGraph.value.setHeightContent();                                // height="content"
    mObjects.tempGraph.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);                 // text-align="right"
    mObjects.tempGraph.value.setAlign(LV_ALIGN_TOP_RIGHT, -4, 4);               // align="top-right -4px 4px"
    mObjects.tempGraph.value.setBgColor(lv_color_hex(0x0d141a));                // bg-color="#0D141A"
    mObjects.tempGraph.value.setBgOpa(204);                                     // bg-opa="80%"
    mObjects.tempGraph.value.setRadius(3);                                      // radius="3px"
    mObjects.powerGraph.body.setLayout(LV_LAYOUT_FLEX);                         // layout="flex"
    mObjects.powerGraph.body.setFlexFlow(LV_FLEX_FLOW_ROW);                     // flex-flow="row"
    mObjects.powerGraph.body.setWidth(LV_PCT(100));                             // width="100%"
    mObjects.powerGraph.body.setHeight(66);                                     // height="66px"
    mObjects.powerGraph.labelContainer.setWidth(18);                            // width="18px"
    mObjects.powerGraph.labelContainer.setHeight(LV_PCT(100));                  // height="100%"
    mObjects.powerGraph.labelContainer.setLayout(LV_LAYOUT_FLEX);               // layout="flex"
    mObjects.powerGraph.labelContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);        // flex-flow="column"
    mObjects.powerGraph.labelContainer.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.powerGraph._namelessContainer_0.setFlexGrow(1);                    // flex-grow="1"
    mObjects.powerGraph._namelessContainer_0.setHeight(LV_PCT(100));            // height="100%"
    mObjects.powerGraph._namelessContainer_0.setBgColor(lv_color_hex(0x0d141a)); // bg-color="#0D141A"
    mObjects.powerGraph._namelessContainer_0.setBgOpa(255);                     // bg-opa="100%"
    mObjects.powerGraph._namelessContainer_0.setBorderColor(lv_color_hex(0x26343d)); // border-color="#26343D"
    mObjects.powerGraph._namelessContainer_0.setBorderWidth(1);                 // border-width="1px"
    mObjects.powerGraph._namelessContainer_0.setBorderSide(LV_BORDER_SIDE_FULL); // border-side="full"
    mObjects.powerGraph._namelessContainer_0.setRadius(7);                      // radius="7px"
    mObjects.powerGraph.chart.setWidth(LV_PCT(100));                            // width="100%"
    mObjects.powerGraph.chart.setHeight(LV_PCT(100));                           // height="100%"
    mObjects.powerGraph.value.setText("");                                      // text=""
    mObjects.powerGraph.value.setWidthContent();                                // width="content"
    mObjects.powerGraph.value.setHeightContent();                               // height="content"
    mObjects.powerGraph.value.setTextAlign(LV_TEXT_ALIGN_RIGHT);                // text-align="right"
    mObjects.powerGraph.value.setAlign(LV_ALIGN_TOP_RIGHT, -4, 4);              // align="top-right -4px 4px"
    mObjects.powerGraph.value.setBgColor(lv_color_hex(0x0d141a));               // bg-color="#0D141A"
    mObjects.powerGraph.value.setBgOpa(204);                                    // bg-opa="80%"
    mObjects.powerGraph.value.setRadius(3);                                     // radius="3px"
    mObjects.timeAxis.setWidth(LV_PCT(100));                                    // width="100%"
    mObjects.timeAxis.setFlexGrow(1);                                           // flex-grow="1"
    mObjects.timeAxis.setLayout(LV_LAYOUT_FLEX);                                // layout="flex"
    mObjects.timeAxis.setFlexFlow(LV_FLEX_FLOW_ROW);                            // flex-flow="row"
    mObjects.timeAxis.setFlexAlign(LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="space-around center center"
    mObjects.timeAxis.setPadLeft(18);                                           // pad-left="18px"
    mObjects.t4.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t4.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t4.setTextColor(lv_color_hex(0x60717b));                           // text-color="#60717B"
    mObjects.t4.setText("60s");                                                 // text="60s"
    mObjects.t3.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t3.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t3.setTextColor(lv_color_hex(0x60717b));                           // text-color="#60717B"
    mObjects.t3.setText("45s");                                                 // text="45s"
    mObjects.t2.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t2.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t2.setTextColor(lv_color_hex(0x60717b));                           // text-color="#60717B"
    mObjects.t2.setText("30s");                                                 // text="30s"
    mObjects.t1.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t1.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t1.setTextColor(lv_color_hex(0x60717b));                           // text-color="#60717B"
    mObjects.t1.setText("15s");                                                 // text="15s"
    mObjects.t0.setWidth(LV_PCT(20));                                           // width="20%"
    mObjects.t0.setTextAlign(LV_TEXT_ALIGN_CENTER);                             // text-align="center"
    mObjects.t0.setTextColor(lv_color_hex(0x8a9aa4));                           // text-color="#8A9AA4"
    mObjects.t0.setText("NOW");                                                 // text="NOW"
    mObjects.controlPanel.setLayout(LV_LAYOUT_FLEX);                            // layout="flex"
    mObjects.controlPanel.setFlexFlow(LV_FLEX_FLOW_COLUMN);                     // flex-flow="column"
    mObjects.controlPanel.setFlexGrow(1);                                       // flex-grow="1"
    mObjects.controlPanel.setHeight(LV_PCT(100));                               // height="100%"
    mObjects.controlPanel.setPadGap(4);                                         // pad-gap="4px"
    mObjects.systemStateBg.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.systemStateBg.setHeight(24);                                       // height="24px"
    mObjects.systemStateBg.setBgColor(lv_color_hex(0x7bbf56));                  // bg-color="#7BBF56"
    mObjects.systemStateBg.setBgOpa(255);                                       // bg-opa="100%"
    mObjects.systemStateBg.setRadius(8);                                        // radius="8px"
    mObjects.systemState.setAlign(LV_ALIGN_CENTER, 0, 0);                       // align="center"
    mObjects.systemState.setText("READY");                                      // text="READY"
    mObjects.systemState.setTextColor(lv_color_hex(0x071008));                  // text-color="#071008"
    mObjects.controlHeader.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.controlHeader.setHeight(14);                                       // height="14px"
    mObjects.controlHeader.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.controlHeader.setFlexFlow(LV_FLEX_FLOW_ROW);                       // flex-flow="row"
    mObjects.controlHeader.setFlexAlign(LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="space-between center center"
    mObjects.controlsTitle.setText("MENU");                                     // text="MENU"
    mObjects.controlsTitle.setTextColor(lv_color_hex(0x778791));                // text-color="#778791"
    mObjects.modeLabel.setText("BROWSE");                                       // text="BROWSE"
    mObjects.modeLabel.setTextColor(lv_color_hex(0x8ab4f4));                    // text-color="#8AB4F4"
    mObjects.menuContainer.setWidth(LV_PCT(100));                               // width="100%"
    mObjects.menuContainer.setFlexGrow(1);                                      // flex-grow="1"
    mObjects.menuContainer.setLayout(LV_LAYOUT_FLEX);                           // layout="flex"
    mObjects.menuContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);                    // flex-flow="column"
    mObjects.menuContainer.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center"
    mObjects.menuArrUp.setSource(&MainScreen_image_arrow_up);                   // src="arrow-up"
    mObjects.menuArrUp.setOpa(128);                                             // opa="50%"
    mObjects.menuRows.setWidth(LV_PCT(100));                                    // width="100%"
    mObjects.menuRows.setHeightContent();                                       // height="content"
    mObjects.menuArrDown.setSource(&MainScreen_image_arrow_down);               // src="arrow-down"
    mObjects.menuArrDown.setOpa(128);                                           // opa="50%"
    mObjects.infoTiles.setWidth(LV_PCT(100));                                   // width="100%"
    mObjects.infoTiles.setHeight(40);                                           // height="40px"
    mObjects.infoTiles.setLayout(LV_LAYOUT_FLEX);                               // layout="flex"
    mObjects.infoTiles.setFlexFlow(LV_FLEX_FLOW_ROW_WRAP);                      // flex-flow="row-wrap"
    mObjects.infoTiles.setFlexAlign(LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_SPACE_AROUND); // flex-align="space-around space-around space-around"
    mObjects.infoTiles.setBgColor(lv_color_hex(0x0d141a));                      // bg-color="#0D141A"
    mObjects.infoTiles.setBgOpa(255);                                           // bg-opa="100%"
    mObjects.infoTiles.setRadius(6);                                            // radius="6px"
    mObjects.infoTiles.setPad(2, 2, 2, 2);                                      // pad="2px"
    mObjects.fanInfo.body.setLayout(LV_LAYOUT_FLEX);                            // layout="flex"
    mObjects.fanInfo.body.setFlexFlow(LV_FLEX_FLOW_ROW);                        // flex-flow="row"
    mObjects.fanInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.fanInfo.body.setPadGap(3);                                         // pad-gap="3px"
    mObjects.fanInfo.body.setWidth(LV_PCT(50));                                 // width="50%"
    mObjects.fanInfo.body.setHeight(LV_PCT(50));                                // height="50%"
    mObjects.fanInfo.icon.setSource(&MainScreen_image_fan16);                   // src="fan16"
    mObjects.fanInfo.icon.setWidth(16);                                         // width="16px"
    mObjects.fanInfo.icon.setHeight(16);                                        // height="16px"
    mObjects.fanInfo.icon.setOpa(166);                                          // opa="65%"
    mObjects.fanInfo.value.setText("OFF");                                      // text="OFF"
    mObjects.fanInfo.value.setTextAlign(LV_TEXT_ALIGN_LEFT);                    // text-align="left"
    mObjects.humidInfo.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.humidInfo.body.setFlexFlow(LV_FLEX_FLOW_ROW);                      // flex-flow="row"
    mObjects.humidInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.humidInfo.body.setPadGap(3);                                       // pad-gap="3px"
    mObjects.humidInfo.body.setWidth(LV_PCT(50));                               // width="50%"
    mObjects.humidInfo.body.setHeight(LV_PCT(50));                              // height="50%"
    mObjects.humidInfo.icon.setSource(&MainScreen_image_humidity16);            // src="humidity16"
    mObjects.humidInfo.icon.setWidth(16);                                       // width="16px"
    mObjects.humidInfo.icon.setHeight(16);                                      // height="16px"
    mObjects.humidInfo.icon.setOpa(166);                                        // opa="65%"
    mObjects.humidInfo.value.setText("--%");                                    // text="--%"
    mObjects.humidInfo.value.setTextAlign(LV_TEXT_ALIGN_LEFT);                  // text-align="left"
    mObjects.powerInfo.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.powerInfo.body.setFlexFlow(LV_FLEX_FLOW_ROW);                      // flex-flow="row"
    mObjects.powerInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.powerInfo.body.setPadGap(3);                                       // pad-gap="3px"
    mObjects.powerInfo.body.setWidth(LV_PCT(50));                               // width="50%"
    mObjects.powerInfo.body.setHeight(LV_PCT(50));                              // height="50%"
    mObjects.powerInfo.icon.setSource(&MainScreen_image_power16);               // src="power16"
    mObjects.powerInfo.icon.setWidth(16);                                       // width="16px"
    mObjects.powerInfo.icon.setHeight(16);                                      // height="16px"
    mObjects.powerInfo.icon.setOpa(166);                                        // opa="65%"
    mObjects.powerInfo.value.setText("0.0W");                                   // text="0.0W"
    mObjects.powerInfo.value.setTextAlign(LV_TEXT_ALIGN_LEFT);                  // text-align="left"
    mObjects.deltaInfo.body.setLayout(LV_LAYOUT_FLEX);                          // layout="flex"
    mObjects.deltaInfo.body.setFlexFlow(LV_FLEX_FLOW_ROW);                      // flex-flow="row"
    mObjects.deltaInfo.body.setFlexAlign(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // flex-align="center center center"
    mObjects.deltaInfo.body.setPadGap(3);                                       // pad-gap="3px"
    mObjects.deltaInfo.body.setWidth(LV_PCT(50));                               // width="50%"
    mObjects.deltaInfo.body.setHeight(LV_PCT(50));                              // height="50%"
    mObjects.deltaInfo.icon.setSource(&MainScreen_image_temperature16);         // src="temperature16"
    mObjects.deltaInfo.icon.setWidth(16);                                       // width="16px"
    mObjects.deltaInfo.icon.setHeight(16);                                      // height="16px"
    mObjects.deltaInfo.icon.setOpa(166);                                        // opa="65%"
    mObjects.deltaInfo.value.setText("--.-°");                                  // text="--.-°"
    mObjects.deltaInfo.value.setTextAlign(LV_TEXT_ALIGN_LEFT);                  // text-align="left"
    mObjects.footer.setWidth(LV_PCT(100));                                      // width="100%"
    mObjects.footer.setHeight(20);                                              // height="20px"
    mObjects.pressHint.setText("PRESS: EDIT/OPEN");                             // text="PRESS: EDIT/OPEN"
    mObjects.holdHint.setText("HOLD: HOME");                                    // text="HOLD: HOME"
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
    chartsPanel(wrapper),
    chartsHeader(chartsPanel),
    trendsTitle(chartsHeader),
    windowLabel(chartsHeader),
    tempGraph(chartsPanel),
    powerGraph(chartsPanel),
    timeAxis(chartsPanel),
    t4(timeAxis),
    t3(timeAxis),
    t2(timeAxis),
    t1(timeAxis),
    t0(timeAxis),
    controlPanel(wrapper),
    systemStateBg(controlPanel),
    systemState(systemStateBg),
    controlHeader(controlPanel),
    controlsTitle(controlHeader),
    modeLabel(controlHeader),
    menuContainer(controlPanel),
    menuArrUp(menuContainer),
    menuRows(menuContainer),
    menuArrDown(menuContainer),
    infoTiles(controlPanel),
    fanInfo(infoTiles),
    humidInfo(infoTiles),
    powerInfo(infoTiles),
    deltaInfo(infoTiles),
    footer(root),
    pressHint(footer),
    holdHint(footer){}

} // namespace

