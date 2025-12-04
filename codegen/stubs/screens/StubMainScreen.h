#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/MainScreenAbs.h"

namespace Garbox {

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

    // event handlers
    virtual void onDisplayCommand(const DisplayCommandEvent& event) = 0;
    virtual void onFanCommand(const FanCommandEvent& event) = 0;
    virtual void onHeatpadCommand(const HeatpadCommandEvent& event) = 0;

private:

    // model field callbacks to update specific parts of the screen
    void onApplyFanState() final;
    void onApplyFanTargetSpeed() final;
    void onApplyFanMeasuredRpm() final;
    void onApplyHeatpadState() final;
    void onApplyHeatpadCurrentDuty() final;
    void onApplyHeatpadMeasuredVoltage() final;
    void onApplyHeatpadMeasuredCurrent() final;
    void onApplyDisplayBrightness() final;
    void onApplyShtDriverEnabled() final;
    void onApplyShtPowerEnabled() final;
    void onApplyShtResetting() final;
    void onApplyMeasuredTemperature() final;
    void onApplyMeasuredHumidity() final;
    void onApplyTargetTemperature() final;
    void onApplyEngineState() final;

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; 

};

} // namespace Garbox