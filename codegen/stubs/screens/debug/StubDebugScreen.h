#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/DebugScreenAbs.h"

namespace Garbox {

class DebugScreen : public DebugScreenAbs {
public:

    DebugScreen();

private:

    // model field callbacks to update specific parts of the screen
    void onApplyFanState() final;
    void onApplyFanMeasuredRpm() final;
    void onApplyHeatpadState() final;
    void onApplyHeatpadDuty() final;
    void onApplyBoxPosition() final;
    void onApplyHeatpadSense() final;
    void onApplyDisplayStatus() final;
    void onApplyTemperatureState() final;
    void onApplyTemperatureSample() final;
    void onApplyAppInfo() final;
    void onApplyFermentationStatus() final;
    void onApplyHeapSpace() final;

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; 

};

} // namespace Garbox