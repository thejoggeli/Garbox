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
    void onApplyShtDriverEnabled() final;
    void onApplyShtPowerEnabled() final;
    void onApplyShtResetting() final;
    void onApplyMeasuredTemperature() final;
    void onApplyMeasuredHumidity() final;
    void onApplyTargetTemperature() final;
    void onApplyEngineState() final;

private:

    LvImage mHeatImage;
    LvImage mFanImage;
    LvImage mTemperatureImage;
    LvImage mHumidityImage;
    LvImage mPowerImage;

    LvLabel mHeatLabel;
    LvLabel mFanLabel;
    LvLabel mTemperatureLabel;
    LvLabel mHumidityLabel;
    LvLabel mPowerLabel;

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final;

};

} // namespace Garbox