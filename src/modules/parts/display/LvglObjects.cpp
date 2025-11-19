
#include "LvglObjects.h"

#include <cstdio>
#include <lvgl.h>
#include "core/assert/Assert.h"

namespace Garbox {

LvglObjects::LvglObjects(){ 
    // constructor body
}

void LvglObjects::init(lv_obj_t *parentObject){

    // initialization check
    AssertExit((parentObject != nullptr), "LvglObjects", "parentObject is null");

    // layout parameters
    const int16_t startXPx = 10;
    const int16_t startYPx = 10;
    const int16_t deltaYPx = 20;

    int16_t currentYPx = startYPx;

    // create fan state label
    mFanStateLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mFanStateLabel, startXPx, currentYPx);
    lv_label_set_text(mFanStateLabel, "Fan state: ");
    currentYPx = static_cast<int16_t>(currentYPx + deltaYPx);

    // create fan target speed label
    mFanTargetSpeedLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mFanTargetSpeedLabel, startXPx, currentYPx);
    lv_label_set_text(mFanTargetSpeedLabel, "Fan target speed: ");
    currentYPx = static_cast<int16_t>(currentYPx + deltaYPx);

    // create fan measured rpm label
    mFanMeasuredRpmLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mFanMeasuredRpmLabel, startXPx, currentYPx);
    lv_label_set_text(mFanMeasuredRpmLabel, "Fan measured rpm: ");
    currentYPx = static_cast<int16_t>(currentYPx + deltaYPx * 2);

    // create heatpad state label
    mHeatpadStateLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mHeatpadStateLabel, startXPx, currentYPx);
    lv_label_set_text(mHeatpadStateLabel, "Heatpad state: ");
    currentYPx = static_cast<int16_t>(currentYPx + deltaYPx);

    // create heatpad duty label
    mHeatpadDutyLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mHeatpadDutyLabel, startXPx, currentYPx);
    lv_label_set_text(mHeatpadDutyLabel, "Heatpad duty: ");
    currentYPx = static_cast<int16_t>(currentYPx + deltaYPx);

    // create heatpad current label
    mHeatpadCurrentLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mHeatpadCurrentLabel, startXPx, currentYPx);
    lv_label_set_text(mHeatpadCurrentLabel, "Heatpad current: ");
    currentYPx = static_cast<int16_t>(currentYPx + deltaYPx);

    // create heatpad voltage label
    mHeatpadVoltageLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mHeatpadVoltageLabel, startXPx, currentYPx);
    lv_label_set_text(mHeatpadVoltageLabel, "Heatpad voltage: ");
    currentYPx = static_cast<int16_t>(currentYPx + deltaYPx*2);

    // create heatpad voltage label
    mRenderSkippedCountLabel = lv_label_create(parentObject);
    lv_obj_set_pos(mRenderSkippedCountLabel, startXPx, currentYPx);
    lv_label_set_text(mRenderSkippedCountLabel, "Render skipped count: 0");
}

void LvglObjects::setFanState(const char *stateText){
    // update fan state
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Fan state: %s", stateText);
    lv_label_set_text(mFanStateLabel, buffer);
}

void LvglObjects::setFanTargetSpeed(float targetSpeed01){
    // clamp and update fan target speed
    if(targetSpeed01 < 0.0f){
        targetSpeed01 = 0.0f;
    }
    else if(targetSpeed01 > 1.0f){
        targetSpeed01 = 1.0f;
    }

    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Fan target speed: %.2f", targetSpeed01);
    lv_label_set_text(mFanTargetSpeedLabel, buffer);
}

void LvglObjects::setFanMeasuredRpm(float rpmValue){
    // clamp and update fan rpm
    if(rpmValue < 0.0f){
        rpmValue = 0.0f;
    }
    else if(rpmValue > 5000.0f){
        rpmValue = 5000.0f;
    }

    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Fan measured rpm: %.0f", rpmValue);
    lv_label_set_text(mFanMeasuredRpmLabel, buffer);
}

void LvglObjects::setHeatpadState(const char *stateText){
    // update heatpad state
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Heatpad state: %s", stateText);
    lv_label_set_text(mHeatpadStateLabel, buffer);
}

void LvglObjects::setHeatpadDuty(float duty01){
    // clamp and update heatpad duty
    if(duty01 < 0.0f){
        duty01 = 0.0f;
    }
    else if(duty01 > 1.0f){
        duty01 = 1.0f;
    }

    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Heatpad duty: %.2f", duty01);
    lv_label_set_text(mHeatpadDutyLabel, buffer);
}

void LvglObjects::setHeatpadCurrent(float currentAmps){
    // update heatpad current
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Heatpad current: %.2f A", currentAmps);
    lv_label_set_text(mHeatpadCurrentLabel, buffer);
}

void LvglObjects::setHeatpadVoltage(float voltageVolts){
    // update heatpad voltage
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Heatpad voltage: %.2f V", voltageVolts);
    lv_label_set_text(mHeatpadVoltageLabel, buffer);
}

void LvglObjects::setRenderSkippedCount(uint32_t count){
    // update heatpad voltage
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Render skipped count: %u", count);
    lv_label_set_text(mRenderSkippedCountLabel, buffer);
}

} // namespace Garbox
