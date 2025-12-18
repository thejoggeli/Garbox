#pragma once

#include "app/generated/screens/MainScreenAbs.h"
#include "core/lvgl/helpers/chart/ChartGridRenderer.h"
#include "core/lvgl/objects/LvBox.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

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

    SoftwareTimer mPowerTimer;

    LvImage mTempLabel;
    LvImage mPowerLabel;

    ChartGridRenderer mTempGrid;
    ChartGridRenderer mPowerGrid;
    
    LvChartSeries* mTempSeries = nullptr;
    LvChartSeries* mTempTargetSeries = nullptr;
    LvChartSeries* mPowerSeries = nullptr;

    void initTemperatureChart();
    void initPowerChart();

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