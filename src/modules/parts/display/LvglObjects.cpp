#include "LvglObjects.h"

#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <lvgl.h>
#include "core/assert/Assert.h"

namespace Garbox {

LvglObjects::LvglObjects() {}

// helper to create a label with position and text
lv_obj_t* LvglObjects::createLabel(lv_obj_t* parent, int16_t x, int16_t y, const char* text) {
    lv_obj_t* lbl = lv_label_create(parent);
    lv_obj_set_pos(lbl, x, y);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    setFormatted(lbl, "%s -", text);
    return lbl;
}

// helper to set formatted text on a label
void LvglObjects::setFormatted(lv_obj_t* lbl, const char* fmt, ...) {
    char buffer[64];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    lv_label_set_text(lbl, buffer);
}

void LvglObjects::init(lv_obj_t *parentObject){

    AssertExit((parentObject != nullptr), "LvglObjects", "parentObject is null");

    // disable scrollbar
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP);
    lv_obj_add_style(parentObject, &style, LV_PART_SCROLLBAR);

    // create background
    mBackground = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBackground, 320, 240);
    lv_obj_set_style_bg_color(mBackground, lv_color_hex(0x0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBackground, LV_OPA_COVER, LV_PART_MAIN);

    const int16_t startXPx = 10;
    const int16_t startYPx = 10;
    const int16_t deltaYPx = 20;

    int16_t currentYPx = startYPx;

    mAppInfoLabel            = createLabel(parentObject, startXPx, currentYPx, "App:"); currentYPx += deltaYPx;
    mHeapSpaceLabel          = createLabel(parentObject, startXPx, currentYPx, "Heap space:"); currentYPx += deltaYPx;
    mDisplayStateLabel       = createLabel(parentObject, startXPx, currentYPx, "Render skipped count:"); currentYPx += deltaYPx;
    mFermentationStatusLabel = createLabel(parentObject, startXPx, currentYPx, "Eng:"); currentYPx += deltaYPx;
    mTemperatureStateLabel   = createLabel(parentObject, startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    mTemperatureSampleLabel  = createLabel(parentObject, startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    mFanStateLabel           = createLabel(parentObject, startXPx, currentYPx, "Fan state"); currentYPx += deltaYPx;
    mFanMeasuredRpmLabel     = createLabel(parentObject, startXPx, currentYPx, "Fan"); currentYPx += deltaYPx;
    mHeatpadStateLabel       = createLabel(parentObject, startXPx, currentYPx, "Heatpad"); currentYPx += deltaYPx;
    mHeatpadSenseLabel       = createLabel(parentObject, startXPx, currentYPx, "Heatpad");currentYPx += deltaYPx;
    mHeatpadDutyLabel        = createLabel(parentObject, startXPx, currentYPx, "Duty%:"); currentYPx += deltaYPx;

    // create box1
    mBox = lv_obj_create(lv_scr_act());
    lv_obj_set_size(mBox, 48, 8);
    lv_obj_set_style_bg_color(mBox, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mBox, LV_OPA_COVER, LV_PART_MAIN);
}

void LvglObjects::setFanState(const char *stateText, float targetSpeed){
    setFormatted(mFanStateLabel, "Fan state=%s, speed=%.1f%%", stateText, targetSpeed*100.0f);
}

void LvglObjects::setFanMeasuredRpm(float rpmValue){
    setFormatted(mFanMeasuredRpmLabel, "Fan rpm=%.0f", rpmValue);
}

void LvglObjects::setHeatpadState(const char *stateText){
    setFormatted(mHeatpadStateLabel, "Heatpad state=%s", stateText);
}

void LvglObjects::setHeatpadDuty(float currentDuty, float nextDuty, uint32_t currentPeriodMicros, uint32_t nextPeriodMicros){
    setFormatted(mHeatpadDutyLabel, "Duty%: %.0f%% => %.0f%%, ms: %u => %u", 
        currentDuty*100.0f,
        nextDuty*100.0f,
        currentPeriodMicros/1000,
        nextPeriodMicros/1000
    );
}

void LvglObjects::setHeatpadSense(float voltageVolts, float currentAmps){
    setFormatted(mHeatpadSenseLabel, "Heatpad U=%4.1fV, I=%3.1fA", voltageVolts, currentAmps);
}

void LvglObjects::setDisplayState(float brightness, uint32_t skipped, uint32_t dirtyCount){
    setFormatted(mDisplayStateLabel, "Display: b=%.1f%%, skip=%u, dirty=%u", brightness*100.0f, skipped, dirtyCount);
}

void LvglObjects::setTemperatureState(bool power, bool driver, bool reset){
    setFormatted(mTemperatureStateLabel, "Sht31 power=%u, driver=%u, reset=%u", power, driver, reset);
}

void LvglObjects::setTemperatureSample(float t, float h){
    setFormatted(mTemperatureSampleLabel, "Sht31 t=%.2f°C, rh=%.2f%%", t, h);
}

void LvglObjects::setHeapSpace(uint32_t space){
    setFormatted(mHeapSpaceLabel, "Heap space: %u.%03u kB", space/1000, space%1000);
}

void LvglObjects::setAppInfo(const char* behaviour, uint32_t eventCount){
    setFormatted(mAppInfoLabel, "App: %s, events=%u", behaviour, eventCount);
}

void LvglObjects::setFermentationStatus(const char* engineState, float measuredTemp, float targetTemp){
    setFormatted(mFermentationStatusLabel, "Eng: s=%s, m=%.1f°C, t=%0.1f°C", engineState, measuredTemp, targetTemp);
}

void LvglObjects::setBoxPosition(float position){

    static constexpr uint32_t y = 240-8;
    static constexpr float wDisplay = 320.0f; 
    static constexpr float wBox = 48.0f;
    static constexpr float left = -wBox;
    static constexpr float right = wDisplay;
    const float x = std::clamp(position * (right - left) + left, left, right);
    lv_obj_set_pos(mBox, x, y);
}

void LvglObjects::setBackgroundColor(uint32_t color){
    lv_obj_set_style_bg_color(mBackground, lv_color_hex(color), LV_PART_MAIN);
}

} // namespace Garbox
