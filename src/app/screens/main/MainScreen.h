#pragma once

#include "app/screens/abstract/MainScreenAbs.h"
#include "core/lvgl/objects/LvBox.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"

namespace Garbox {

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

    // event handlers (to be implmeneted by user)
    void onDisplayCommand(const DisplayCommandEvent& event) final;
    void onFanCommand(const FanCommandEvent& event) final;
    void onHeatpadCommand(const HeatpadCommandEvent& event) final;

    // model field callbacks to update specific parts of the screen
    void onApplyFanStatus() final;
    void onApplyFanTargetSpeed() final;
    void onApplyHeatpadStatus() final;
    void onApplyHeatpadMeasure() final;
    void onApplyDisplayBrightness() final;
    void onApplySensorStatus() final;
    void onApplyMeasuredTemperature() final;
    void onApplyMeasuredHumidity() final;
    void onApplyTargetTemperature() final;
    void onApplyEngineState() final;

private:

    LvImage mTemperatureLabel;
    LvImage mPowerLabel;

    bool isSensorOk();
    const char* resovleEngineStateText();
    uint32_t resovleEngineStateColor();
    const char* resovleSensorText();

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final;

};

} // namespace Garbox