#pragma once

#include "app/screens/abstract/MainScreenAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class LvglObjects;

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

private:

    bool mLastDispatchedCount = 0;
    bool mFirstUpdate = true;
    LvglObjects& mObjects;
    SoftwareTimer mHeapTimer;

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    void onUpdateScreen() final;

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

};

} // namespace Garbox