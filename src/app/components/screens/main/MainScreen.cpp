#include "MainScreen.h"

#include <math.h>
#include "app/services/GarboxHistory.h"
#include "core/log/Log.h"
#include "core/lvgl/helpers/RotationRenderer.h"
#include "core/util/function/default/MathFunctions.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {


static constexpr uint32_t ColorRed = 0xB85450;
static constexpr uint32_t ColorBlue = 0x8AB4F4;
static constexpr uint32_t ColorGreen = 0x7BBF56;
static constexpr uint32_t ColorTarget = 0xD1B254;

static constexpr uint32_t ChartSeriesLineWidth = 2;
static constexpr uint32_t GridTicksCountX = 5;
static constexpr uint32_t GridTicksCountY = 3;
static constexpr uint32_t GridLineColor = 0x101010;
static constexpr uint32_t GridLineWidth = 1;

static constexpr uint32_t ChartsPointCount = GarboxHistory::SampleCount;

static constexpr int32_t TempChartYScale = GarboxHistory::TemperaturScaleFactor;
static constexpr int32_t TempChartYMin = TempChartYScale * 16;
static constexpr int32_t TempChartYMax = TempChartYScale * 44;
 
static constexpr int32_t PowerChartYScale = GarboxHistory::PowerScaleFactor;
static constexpr int32_t PowerChartYMin = PowerChartYScale * (0 - 20);
static constexpr int32_t PowerChartYMax = PowerChartYScale * (100 + 20);

MainScreen::MainScreen() : 
    MainScreenAbs(),
    mTempLabel(RotationRenderer::createRotatedLabelRgb565(gui().tempGraph.labelContainer, {
        .text = "Temperature",
        .font = &lv_font_montserrat_14,
        .color = ColorBlue,
        .bgColor = 0x0,
        .angle = RotationRenderer::Angle::Deg270,
    })),
    mPowerLabel(RotationRenderer::createRotatedLabelRgb565(gui().powerGraph.labelContainer, {
        .text = "Power",
        .font = &lv_font_montserrat_14,
        .color = ColorRed,
        .bgColor = 0x0,
        .angle = RotationRenderer::Angle::Deg270,
    })),
    mTempGrid(
        gui().tempGraph.chart,
        GridTicksCountX,
        GridTicksCountY,
        lv_color_hex(GridLineColor),
        GridLineWidth
    ),
    mPowerGrid(
        gui().powerGraph.chart,
        GridTicksCountX,
        GridTicksCountY,
        lv_color_hex(GridLineColor),
        GridLineWidth
    ){}

void MainScreen::onInit(){

    // setup time axis labels
    const lv_font_t* timeAxisFont = &lv_font_montserrat_10; 
    gui().t0.setFont(timeAxisFont);
    gui().t1.setFont(timeAxisFont);
    gui().t2.setFont(timeAxisFont);
    gui().t3.setFont(timeAxisFont);
    gui().t4.setFont(timeAxisFont);

    // setup graph values
    gui().tempGraph.value.setTextColor(lv_color_hex(ColorBlue));
    gui().powerGraph.value.setTextColor(lv_color_hex(ColorRed));
    gui().powerGraph.value.setAlign(LV_ALIGN_BOTTOM_RIGHT, -4, -4);

    // setup info labels
    const lv_font_t* infoLabelFont = &lv_font_montserrat_12; 
    gui().fanInfo.value.setFont(infoLabelFont);
    gui().timeInfo.value.setFont(infoLabelFont);
    gui().humidInfo.value.setFont(infoLabelFont);
    gui().powerInfo.value.setFont(infoLabelFont);

    // setup menu
    const lv_font_t* menuFont = &lv_font_montserrat_12;
    gui().menuContainer.setFont(menuFont);

    // init charts
    initTemperatureChart();
    initPowerChart();

    const int32_t pointIndexLeft = ChartsPointCount * 0.1f;
    const int32_t pointIndexRight = ChartsPointCount * 0.9f;

    // temperature horizontal grid
    mTempGrid.setYTickPosition(0, MathUtils::Normalize(20 * TempChartYScale, TempChartYMin, TempChartYMax));
    mTempGrid.setYTickPosition(1, MathUtils::Normalize(30 * TempChartYScale, TempChartYMin, TempChartYMax));
    mTempGrid.setYTickPosition(2, MathUtils::Normalize(40 * TempChartYScale, TempChartYMin, TempChartYMax));
    mTempGrid.setYTickLabel(0, "20°C");
    mTempGrid.setYTickLabel(1, "30°C");
    mTempGrid.setYTickLabel(2, "40°C");

    // power horizontal grid
    mPowerGrid.setYTickPosition(0, MathUtils::Normalize(0   * TempChartYScale, PowerChartYMin, PowerChartYMax));
    mPowerGrid.setYTickPosition(1, MathUtils::Normalize(50  * TempChartYScale, PowerChartYMin, PowerChartYMax));
    mPowerGrid.setYTickPosition(2, MathUtils::Normalize(100 * TempChartYScale, PowerChartYMin, PowerChartYMax));
    mPowerGrid.setYTickLabel(0, "0%");
    mPowerGrid.setYTickLabel(1, "50%");
    mPowerGrid.setYTickLabel(2, "100%");

    // charts vertical grid
    for(int32_t i = 0; i < GridTicksCountX; i++){
        float pointIndex = MathUtils::Map<float>(i, 0, GridTicksCountX-1, pointIndexLeft, pointIndexRight);
        float relativePos = MathUtils::Normalize(pointIndex, 0, ChartsPointCount);
        mTempGrid.setXTickPosition(i, relativePos);
        mPowerGrid.setXTickPosition(i, relativePos);
    }
}

void MainScreen::initTemperatureChart(){

    // setup temperature chart
    LvChart& chart = gui().tempGraph.chart;
    chart.setType(LV_CHART_TYPE_LINE);
    chart.setPointCount(ChartsPointCount);
    chart.setAxisRange(LV_CHART_AXIS_PRIMARY_Y, TempChartYMin, TempChartYMax);
    chart.setDivLineCount(0, 0);
    chart.setUpdateMode(LV_CHART_UPDATE_MODE_SHIFT);
    chart.setStyleSize(0, 0, LV_PART_INDICATOR); // markers size
    chart.setStyleLineWidth(ChartSeriesLineWidth, LV_PART_ITEMS); // series line width

    // create temperature series
    mTempSeries = chart.addSeries(lv_color_hex(ColorBlue)); 

    // create temperature target series
    mTempTargetSeries = chart.addSeries(lv_color_hex(ColorTarget)); 

    // refresh chart
    chart.refresh();
}

void MainScreen::initPowerChart(){

    // setup temperature chart
    LvChart& chart = gui().powerGraph.chart;
    chart.setType(LV_CHART_TYPE_LINE);
    chart.setPointCount(ChartsPointCount);
    chart.setAxisRange(LV_CHART_AXIS_PRIMARY_Y, PowerChartYMin, PowerChartYMax);
    chart.setDivLineCount(0, 0);
    chart.setUpdateMode(LV_CHART_UPDATE_MODE_SHIFT);
    chart.setStyleSize(0, 0, LV_PART_INDICATOR); // markers size
    chart.setStyleLineWidth(ChartSeriesLineWidth, LV_PART_ITEMS); // series line width

    // create temperature series
    mPowerSeries = chart.addSeries(lv_color_hex(ColorRed)); 

    // refresh chart
    chart.refresh();
}

void MainScreen::onStart(){
    // nothing to do
}

void MainScreen::onBecomeEnabled(){

    GarboxHistory& history = GarboxHistory::Instance();

    // load temperature history
    {
        const TimeSeries& series = history.getMeasuredTemperatureSeries(GarboxHistory::SeriesIndex::Window_01min);

        LvChart& chart = gui().tempGraph.chart;
        chart.resetSeries(mTempSeries);

        auto it = series.iterateOldestToNewest();
        while(it.hasNext()){
            int32_t value = it.next();
            chart.setNextValue(mTempSeries, value);
            LogDebug("MS", "T=%.1f", static_cast<float>(value)/TempChartYScale);
        }
        mTempLastWriteSequence = series.getWriteSequence();
    }

    // load temperature target history
    {
        const TimeSeries& series = history.getTargetTemperatureSeries(GarboxHistory::SeriesIndex::Window_01min);

        LvChart& chart = gui().tempGraph.chart;
        chart.resetSeries(mTempTargetSeries);

        auto it = series.iterateOldestToNewest();
        while(it.hasNext()){
            int32_t value = it.next();
            chart.setNextValue(mTempTargetSeries, value);
            LogDebug("MS", "T=%.1f", static_cast<float>(value)/TempChartYScale);
        }
        mTempTargetLastWriteSequence = series.getWriteSequence();
    }

    // load power history
    {
        const TimeSeries& series = history.getPowerSeries(GarboxHistory::SeriesIndex::Window_01min);

        LvChart& chart = gui().powerGraph.chart;
        chart.resetSeries(mPowerSeries);

        auto it = series.iterateOldestToNewest();
        while(it.hasNext()){
            int32_t value = it.next();
            chart.setNextValue(mPowerSeries, value);
            LogDebug("MS", "P=%.1f", static_cast<float>(value)/PowerChartYScale);
        }
        mPowerLastWriteSequence = series.getWriteSequence();
    }
}

void MainScreen::onBecomeDisabled(){
    // nothing to do
}

void MainScreen::onRender(){

    GarboxHistory& history = GarboxHistory::Instance();

    // update temperature history
    const TimeSeries& tempSeries = history.getMeasuredTemperatureSeries(GarboxHistory::SeriesIndex::Window_01min);
    if(tempSeries.getWriteSequence() != mTempLastWriteSequence){
        auto it = tempSeries.iterateSince(mTempLastWriteSequence);
        while(it.hasNext()){
            gui().tempGraph.chart.setNextValue(mTempSeries, it.next());
        }        
        mTempLastWriteSequence = tempSeries.getWriteSequence();
    }

    // update temperature target history
    const TimeSeries& tempTargetSeries = history.getTargetTemperatureSeries(GarboxHistory::SeriesIndex::Window_01min);
    if(tempTargetSeries.getWriteSequence() != mTempTargetLastWriteSequence){
        auto it = tempTargetSeries.iterateSince(mTempTargetLastWriteSequence);
        while(it.hasNext()){
            gui().tempGraph.chart.setNextValue(mTempTargetSeries, it.next());
        }        
        mTempTargetLastWriteSequence = tempTargetSeries.getWriteSequence();
    }

    // update power history
    const TimeSeries& powerSeries = history.getPowerSeries(GarboxHistory::SeriesIndex::Window_01min);
    if(powerSeries.getWriteSequence() != mPowerLastWriteSequence){
        auto it = powerSeries.iterateSince(mPowerLastWriteSequence);
        while(it.hasNext()){
            gui().powerGraph.chart.setNextValue(mPowerSeries, it.next());
        }        
        mPowerLastWriteSequence = powerSeries.getWriteSequence();
    }

}

bool MainScreen::isSensorOk(){
    const TemperatureStatusState& state = states().temperatureStatus;
    return state.getPowerEnabled() && !state.getResetting() && state.getDriverEnabled() && state.getHasFirstSample();
}

const char* MainScreen::resovleEngineStateText(){
    const FermentationState state = states().fermentationStatus.getState();
    switch(state){
        case FermentationState::Reset:
            return "Reset";
        case FermentationState::Cooldown:
            return "Off";
        case FermentationState::Ready:
            return "Ready";
        case FermentationState::Regulating:
            return "Running";
        case FermentationState::OverTemperature:
            return "Overheat";
        case FermentationState::InvalidInput:
            return "Error #2";
        case FermentationState::Null:
            return "Error #0";
        case FermentationState::Count:
        default:
            return "Error #1";
    }
}

uint32_t MainScreen::resovleEngineStateColor(){
    const FermentationState state = states().fermentationStatus.getState();
    switch(state){
        case FermentationState::Cooldown:
            return ColorBlue; // blue
        case FermentationState::Reset:
        case FermentationState::Ready:
        case FermentationState::Regulating:
            return ColorGreen; // green
        case FermentationState::OverTemperature:
        case FermentationState::InvalidInput:
        case FermentationState::Null:
        case FermentationState::Count:
        default:
            return ColorRed; // red
    }
}

const char* MainScreen::resovleSensorText(){
    const TemperatureStatusState& state = states().temperatureStatus;
    if(state.getResetting()){
        return "Reset";
    }
    else if(!state.getPowerEnabled() || !state.getDriverEnabled()){
        return "Off";
    }
    else if(!state.getHasFirstSample()){
        return "Busy";
    }
    return "Ok";
}

void MainScreen::onRenderFanInfo(){
    FanState state = states().fanStatus.getState();
    if(state == FanState::Stalled){
        gui().fanInfo.value.setText("Stall");
    }
    else {
        float rpm = states().fanSample.getMeasuredRpm();
        uint32_t speed = std::clamp((rpm / 2250.0f) * 100.0f, 0.0f, 100.0f) + 0.5f; 
        gui().fanInfo.value.setTextFormatted("%u%%", speed);
    }
}

void MainScreen::onRenderPowerInfo(){
    const float voltage = states().heatpadSample.getMeasuredVoltage();
    const float current = states().heatpadSample.getMeasuredCurrent();
    const float duty = states().heatpadStatus.getNextDutyCycle();
    const float power = voltage * current * duty;
    gui().powerInfo.value.setTextFormatted("%.1fW", power);
}

void MainScreen::onRenderHumidityInfo(){
    if(isSensorOk()){
        float humidity = states().temperatureSample.getHumidityRelative();
        gui().humidInfo.value.setTextFormatted("%.1f%%", humidity);
    }
    else {
        gui().humidInfo.value.setText(resovleSensorText());
    }
}

void MainScreen::onRenderStatusInfo(){
    gui().systemState.setText(resovleEngineStateText());
    gui().systemStateBg.setBgColor(lv_color_hex(resovleEngineStateColor()));
}

void MainScreen::onRenderMeasuredTemperatureLabel(){
    if(!isSensorOk()){
        gui().tempGraph.value.setTextFormatted(resovleSensorText());
        return;
    }
    float temperature = states().temperatureSample.getTemperatureCelcius();
    gui().tempGraph.value.setTextFormatted("%.1f°C", temperature);
}

void MainScreen::onRenderHeatpadPowerLabel(){
    HeatpadState state = states().heatpadStatus.getState();
    if(state == HeatpadState::Disabled){
        gui().powerGraph.value.setText("Off");
        return;
    }
    const float power = states().heatpadStatus.getNextDutyCycle() * 100.0f;
    gui().powerGraph.value.setTextFormatted("%.1f%%", power);
}

void MainScreen::onFanStatusStateChanged(const FanStatusState& state){
    markDirty(RenderFn::FanInfo);
}

void MainScreen::onFanSampleStateChanged(const FanSampleState& state){
    markDirty(RenderFn::FanInfo);
}

void MainScreen::onHeatpadStatusStateChanged(const HeatpadStatusState& state){
    markDirty(RenderFn::PowerInfo);
    markDirty(RenderFn::HeatpadPowerLabel);
}

void MainScreen::onHeatpadSampleStateChanged(const HeatpadSampleState& state){
    markDirty(RenderFn::PowerInfo);
}

void MainScreen::onTemperatureStatusStateChanged(const TemperatureStatusState& state){
    markDirty(RenderFn::MeasuredTemperatureLabel);
    markDirty(RenderFn::HumidityInfo);
}

void MainScreen::onTemperatureSampleStateChanged(const TemperatureSampleState& state){
    markDirty(RenderFn::MeasuredTemperatureLabel);
    markDirty(RenderFn::HumidityInfo);
}

void MainScreen::onFermentationStatusStateChanged(const FermentationStatusState& state){
    markDirty(RenderFn::StatusInfo);
}

} // namespace Garbox