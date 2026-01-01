#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/screens/main/MainScreenAbs.h"
// ==== GENERATED END: include ====================================================

#include "app/components/screens/main/MainScreenMenu.h"
#include "app/services/GarboxHistory.h"
#include "core/lvgl/helpers/chart/ChartGridRenderer.h"
#include "core/lvgl/helpers/chart/MultiSeriesChart.h"
#include "core/lvgl/objects/LvBox.h"
#include "core/lvgl/objects/LvImage.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class MainScreen : public MainScreenAbs {
private:

    using HistoryIndex = GarboxHistory::SeriesIndex;
    using MenuRowIndex = MainScreenMenu::RowIndex;

    MainScreenMenu mMenu;

    HistoryIndex mHistoryIndex = HistoryIndex::Window_01min;

    SoftwareTimer mMenuTimer;
    SoftwareTimer mAxisTimer;

    LvImage mTempLabel;
    LvImage mPowerLabel;

    MultiSeriesChart mTempChart;
    MultiSeriesChart mPowerChart;
    
    ChartGridRenderer mTempGrid;
    ChartGridRenderer mPowerGrid;

    void initChart(MultiSeriesChart& chart, int32_t yMin, int32_t yMax);

    bool isSensorOk();
    const char* resovleEngineStateText();
    uint32_t resovleEngineStateColor();
    const char* resovleSensorText();

    void setHistoryIndex(HistoryIndex index);

    void handleMenuValueChanged(MenuRowIndex index, int32_t oldValue, int32_t newValue);
    static void menuValueChangedTrampoline(void* ctx, MenuRowIndex index, int32_t oldValue, int32_t newValue){
        static_cast<MainScreen*>(ctx)->handleMenuValueChanged(index, oldValue, newValue);
    }

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    MainScreen();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    // generated tick handlers
    void onUserInputTick() final;

    // generated render functions
    void onRender() final;
    void onRenderFanInfo() final;
    void onRenderPowerInfo() final;
    void onRenderHumidityInfo() final;
    void onRenderStatusInfo() final;
    void onRenderMeasuredTemperatureLabel() final;
    void onRenderHeatpadPowerLabel() final;
    void onRenderTimeAxis() final;
    void onRenderTimeSeries() final;
    void onRenderMenu() final;

    // generated event handlers
    void onButtonEvent(const ButtonEvent& event) final;
    void onEncoderStepEvent(const EncoderStepEvent& event) final;

    // generated state changed handlers
    void onFanStatusStateChanged(const FanStatusState& state) final;
    void onFanSampleStateChanged(const FanSampleState& state) final;
    void onHeatpadStatusStateChanged(const HeatpadStatusState& state) final;
    void onHeatpadSampleStateChanged(const HeatpadSampleState& state) final;
    void onTemperatureStatusStateChanged(const TemperatureStatusState& state) final;
    void onTemperatureSampleStateChanged(const TemperatureSampleState& state) final;
    void onFermentationStatusStateChanged(const FermentationStatusState& state) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox