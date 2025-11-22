#pragma once

#include <cstddef>
#include <cstdint>

struct _lv_obj_t;
typedef struct _lv_obj_t lv_obj_t;

namespace Garbox {

class LvglObjects {
public:
    explicit LvglObjects();

    void init(lv_obj_t *parentObject);

    void setFanState(const char *stateText, float targetSpeed01);
    void setFanMeasuredRpm(float rpmValue);

    void setHeatpadState(const char *stateText);
    void setHeatpadDuty(float duty01);
    void setHeatpadSense(float voltageVolts, float currentAmps);

    void setDisplayState(float brightness, uint32_t skipped);

    void setTemperatureState(bool power, bool driver, bool reset);
    void setTemperatureSample(float t, float h);

    void setHeapSpace(uint32_t free);
    void setAppInfo(const char* behaviour, uint32_t eventCount);

private:
    lv_obj_t* mFanStateLabel = nullptr;
    lv_obj_t* mFanMeasuredRpmLabel = nullptr;
    lv_obj_t* mHeatpadStateLabel = nullptr;
    lv_obj_t* mHeatpadDutyLabel = nullptr;
    lv_obj_t* mHeatpadSenseLabel = nullptr;
    lv_obj_t* mDisplayStateLabel = nullptr;
    lv_obj_t* mTemperatureStateLabel = nullptr;
    lv_obj_t* mTemperatureSampleLabel = nullptr;
    lv_obj_t* mHeapSpaceLabel = nullptr;
    lv_obj_t* mAppInfoLabel = nullptr;

    lv_obj_t* createLabel(lv_obj_t* parent, int16_t x, int16_t y, const char* text);
    void setFormatted(lv_obj_t* lbl, const char* fmt, ...);
};

} // namespace Garbox
