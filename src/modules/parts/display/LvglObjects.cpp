#include "LvglObjects.h"

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
    lv_label_set_text(lbl, text);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
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

    const int16_t startXPx = 10;
    const int16_t startYPx = 10;
    const int16_t deltaYPx = 20;

    int16_t currentYPx = startYPx;

    mFanStateLabel           = createLabel(parentObject, startXPx, currentYPx, "Fan state: "); currentYPx += deltaYPx;
    mFanMeasuredRpmLabel     = createLabel(parentObject, startXPx, currentYPx, "Fan measured rpm: "); currentYPx += deltaYPx;
    mHeatpadStateLabel       = createLabel(parentObject, startXPx, currentYPx, "Heatpad state: "); currentYPx += deltaYPx;
    mHeatpadDutyLabel        = createLabel(parentObject, startXPx, currentYPx, "Heatpad duty:  "); currentYPx += deltaYPx;
    mHeatpadSenseLabel       = createLabel(parentObject, startXPx, currentYPx, "Heatpad sense: ");currentYPx += deltaYPx;
    mDisplayStateLabel       = createLabel(parentObject, startXPx, currentYPx, "Render skipped count:"); currentYPx += deltaYPx;
    mTemperatureStateLabel   = createLabel(parentObject, startXPx, currentYPx, "Sht31 state: "); currentYPx += deltaYPx;
    mTemperatureSampleLabel  = createLabel(parentObject, startXPx, currentYPx, "Sht31 sample:"); currentYPx += deltaYPx;
    mHeapSpaceLabel          = createLabel(parentObject, startXPx, currentYPx, "Heap space:"); currentYPx += deltaYPx;
    mEventsLabel             = createLabel(parentObject, startXPx, currentYPx, "Events:"); currentYPx += deltaYPx;
}

void LvglObjects::setFanState(const char *stateText, float targetSpeed){
    setFormatted(mFanStateLabel, "Fan state: state=%s, speed=%.1f%%", stateText, targetSpeed*100.0f);
}

void LvglObjects::setFanMeasuredRpm(float rpmValue){
    setFormatted(mFanMeasuredRpmLabel, "Fan measured rpm: %.0f", rpmValue);
}

void LvglObjects::setHeatpadState(const char *stateText){
    setFormatted(mHeatpadStateLabel, "Heatpad state: %s", stateText);
}

void LvglObjects::setHeatpadDuty(float duty){
    setFormatted(mHeatpadDutyLabel, "Heatpad duty:  %.1f%%", duty*100.0f);
}

void LvglObjects::setHeatpadSense(float voltageVolts, float currentAmps){
    setFormatted(mHeatpadSenseLabel, "Heatpad sense: %4.1fV, %3.1fA", voltageVolts, currentAmps);
}

void LvglObjects::setDisplayState(float brightness, uint32_t skipped){
    setFormatted(mDisplayStateLabel, "Display: b=%.1f%%, skip=%u", brightness*100.0f, skipped);
}

void LvglObjects::setTemperatureState(bool power, bool driver, bool reset){
    setFormatted(mTemperatureStateLabel, "Sht31 state:  power=%u, driver=%u, reset=%u", power, driver, reset);
}

void LvglObjects::setTemperatureSample(float t, float h){
    setFormatted(mTemperatureSampleLabel, "Sht31 sample: t=%.2f°C, rh=%.2f%%", t, h);
}

void LvglObjects::setHeapSpace(uint32_t space){
    setFormatted(mHeapSpaceLabel, "Heap space: %u.%03u kB", space/1000, space%1000);
}

void LvglObjects::setEvents(uint32_t count){
    setFormatted(mEventsLabel, "Events: count=%u", count);
}

} // namespace Garbox
