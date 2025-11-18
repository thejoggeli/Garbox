#pragma once

#include <cstddef>

struct _lv_obj_t;
typedef struct _lv_obj_t lv_obj_t;

namespace Garbox {

// forward declaration to avoid including lvgl.h in the header

class LvglObjects {
public:
    // constructor
    explicit LvglObjects();

    void init(lv_obj_t *parentObject);

    // fan display setters
    void setFanState(const char *stateText);
    void setFanTargetSpeed(float targetSpeed01);
    void setFanMeasuredRpm(float rpmValue);

    // heatpad display setters
    void setHeatpadState(const char *stateText);
    void setHeatpadDuty(float duty01);
    void setHeatpadCurrent(float currentAmps);
    void setHeatpadVoltage(float voltageVolts);

    // display controller setters
    void setRenderSkippedCount(uint32_t count);

private:
    // LVGL label pointers
    lv_obj_t* mFanStateLabel = nullptr;
    lv_obj_t* mFanTargetSpeedLabel = nullptr;
    lv_obj_t* mFanMeasuredRpmLabel = nullptr;
    lv_obj_t* mHeatpadStateLabel = nullptr;
    lv_obj_t* mHeatpadDutyLabel = nullptr;
    lv_obj_t* mHeatpadCurrentLabel = nullptr;
    lv_obj_t* mHeatpadVoltageLabel = nullptr;
    lv_obj_t* mRenderSkippedCountLabel = nullptr;
};

} // namespace Garbox
