#pragma once

#include "app/screens/abstract/MainScreenAbs.h"
#include "core/lvgl/LvLabel.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class LvglContext;

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

private:

    LvglContext& mLvgl;
    LvLabel mFanStateLabel;
    LvLabel mFanMeasuredRpmLabel;
    LvLabel mHeatpadStateLabel;
    LvLabel mHeatpadDutyLabel;
    LvLabel mHeatpadSenseLabel;
    LvLabel mDisplayStatusLabel;
    LvLabel mTemperatureStateLabel;
    LvLabel mTemperatureSampleLabel;
    LvLabel mHeapSpaceLabel;
    LvLabel mAppInfoLabel;
    LvLabel mFermentationStatusLabel;

    bool mLastDispatchedCount = 0;
    bool mFirstUpdate = true;
    SoftwareTimer mHeapTimer;

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; // safe to update ui elements in this method

    void onApplyFanState() final;
    void onApplyFanMeasuredRpm() final;
    void onApplyHeatpadState() final;
    void onApplyHeatpadDuty() final;
    void onApplyBoxPosition() final;
    void onApplyHeatpadSense() final;
    void onApplyDisplayStatus() final;
    void onApplyTemperatureState() final;
    void onApplyTemperatureSample() final;
    void onApplyHeapSpace() final;
    void onApplyAppInfo() final;
    void onApplyFermentationStatus() final;

    // helper to init a label with position and text
    void initLabel(LvLabel& label, int16_t x, int16_t y, const char* text);

};

} // namespace Garbox