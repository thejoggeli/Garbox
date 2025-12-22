#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/screens/MainScreenAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/lvgl/helpers/chart/ChartGridRenderer.h"
#include "core/lvgl/objects/LvBox.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class MainScreen : public MainScreenAbs {
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

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    MainScreen();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    // generated state changed handlers
    void onFanStatusStateChanged(const FanStatusState& state) final;
    void onFanSampleStateChanged(const FanSampleState& state) final;
    void onHeatpadStatusStateChanged(const HeatpadStatusState& state) final;
    void onHeatpadSampleStateChanged(const HeatpadSampleState& state) final;
    void onTemperatureStatusStateChanged(const TemperatureStatusState& state) final;
    void onTemperatureSampleStateChanged(const TemperatureSampleState& state) final;
    void onFermentationStatusStateChanged(const FermentationStatusState& state) final;

    // generated render functions
    void onRender() final;
    void onRenderFanInfo() final;
    void onRenderPowerInfo() final;
    void onRenderHumidityInfo() final;
    void onRenderStatusInfo() final;
    void onRenderMeasuredTemperatureSample() final;
    void onRenderMeasuredTemperatureLabel() final;
    void onRenderTargetTemperatureSample() final;
    void onRenderHeatpadPowerSample() final;
    void onRenderHeatpadPowerLabel() final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox