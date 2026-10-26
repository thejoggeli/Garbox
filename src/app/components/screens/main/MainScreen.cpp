#include "MainScreen.h"

#include <algorithm>
#include <cstdio>
#include <math.h>
#include "app/components/screens/ScreenCommandBar.h"
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
static constexpr uint32_t GridLineColor = 0x26343D;
static constexpr uint32_t GridLineWidth = 1;

static constexpr uint32_t ChartsPointCount = GarboxHistory::SampleCount;

static constexpr int32_t TempChartYScale = GarboxHistory::TemperaturScaleFactor;
 
static constexpr int32_t PowerChartYScale = GarboxHistory::PowerScaleFactor;
static constexpr int32_t PowerChartYMin = PowerChartYScale * (0 - 20);
static constexpr int32_t PowerChartYMax = PowerChartYScale * (100 + 20);

MainScreen::MainScreen() : 
    MainScreenAbs(),
    mMenu(gui().menuRows, this, menuValueChangedTrampoline),
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
    mTempChart(gui().tempGraph.chart, 2),
    mPowerChart(gui().powerGraph.chart, 1),
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

    gui().trendsTitle.setFont(&lv_font_montserrat_12);
    gui().windowLabel.setFont(&lv_font_unscii_8);
    gui().controlsTitle.setFont(&lv_font_unscii_8);
    gui().modeLabel.setFont(&lv_font_unscii_8);
    ConfigureScreenCommandBar(
        gui().footer,
        gui().pressHint,
        gui().holdHint,
        "PRESS: EDIT/OPEN",
        "HOLD: HOME"
    );

    // setup time axis labels
    const lv_font_t* timeAxisFont = &lv_font_montserrat_10; 
    gui().timeAxis.setFont(timeAxisFont);

    // setup graph values
    gui().tempGraph.value.setTextColor(lv_color_hex(ColorBlue));
    gui().powerGraph.value.setTextColor(lv_color_hex(ColorRed));
    gui().powerGraph.value.setAlign(LV_ALIGN_BOTTOM_RIGHT, -4, -4);

    // setup info labels
    const lv_font_t* infoLabelFont = &lv_font_montserrat_12; 
    gui().infoTiles.setFont(infoLabelFont);
    gui().infoTiles.setScrollable(false);
    lv_obj_set_scrollbar_mode(gui().infoTiles.raw(), LV_SCROLLBAR_MODE_OFF);
    auto disableInfoTileScrolling = [](MainScreenGui::InfoTile& tile){
        tile.body.setScrollable(false);
        lv_obj_set_scrollbar_mode(tile.body.raw(), LV_SCROLLBAR_MODE_OFF);
    };
    disableInfoTileScrolling(gui().fanInfo);
    disableInfoTileScrolling(gui().humidInfo);
    disableInfoTileScrolling(gui().powerInfo);
    disableInfoTileScrolling(gui().deltaInfo);

    // setup menu
    const lv_font_t* menuFont = &lv_font_montserrat_10;
    gui().menuContainer.setFont(menuFont);
    gui().menuArrUp.setOpa(LV_OPA_COVER);
    gui().menuArrDown.setOpa(LV_OPA_COVER);

    // init charts
    initChart(mTempChart, 19 * TempChartYScale, 41 * TempChartYScale);
    initChart(mPowerChart, PowerChartYMin, PowerChartYMax);
    mTempChart.addSeries(ColorBlue);
    mTempChart.addSeries(ColorTarget);
    mPowerChart.addSeries(ColorRed);

    // Temperature range and grid labels are user-selectable from the menu.
    setTemperaturePlotRange(mTemperaturePlotMinimumCelsius, mTemperaturePlotMaximumCelsius);

    // power horizontal grid
    mPowerGrid.setYTickPosition(0, MathUtils::Normalize(0   * PowerChartYScale, PowerChartYMin, PowerChartYMax));
    mPowerGrid.setYTickPosition(1, MathUtils::Normalize(50  * PowerChartYScale, PowerChartYMin, PowerChartYMax));
    mPowerGrid.setYTickPosition(2, MathUtils::Normalize(100 * PowerChartYScale, PowerChartYMin, PowerChartYMax));
    mPowerGrid.setYTickLabel(0, "0%");
    mPowerGrid.setYTickLabel(1, "50%");
    mPowerGrid.setYTickLabel(2, "100%");

    // charts vertical grid
    mTempGrid.distributeXTicksUniform(0.1f, 0.9f);
    mPowerGrid.distributeXTicksUniform(0.1f, 0.9f);
}

void MainScreen::initChart(MultiSeriesChart& chart, int32_t yMin, int32_t yMax){

    // setup chart
    LvChart& lvChart = chart.getLvChart();
    lvChart.setType(LV_CHART_TYPE_LINE);
    lvChart.setPointCount(ChartsPointCount);
    lvChart.setAxisRange(LV_CHART_AXIS_PRIMARY_Y, yMin, yMax);
    lvChart.setDivLineCount(0, 0);
    lvChart.setUpdateMode(LV_CHART_UPDATE_MODE_SHIFT);
    lvChart.setStyleSize(0, 0, LV_PART_INDICATOR); // markers size
    lvChart.setStyleLineWidth(ChartSeriesLineWidth, LV_PART_ITEMS); // series line width

    // refresh chart
    lvChart.refresh();
}

void MainScreen::updateMenuChrome(){
    const bool editing = mMenu.isEditing();
    const uint32_t accent = editing ? ColorTarget : ColorBlue;

    gui().modeLabel.setText(editing ? "EDIT" : "BROWSE");
    gui().modeLabel.setTextColor(lv_color_hex(accent));

    gui().menuArrUp.setStyleImageRecolor(lv_color_hex(accent), LV_PART_MAIN);
    gui().menuArrUp.setStyleImageRecolorOpa(LV_OPA_COVER, LV_PART_MAIN);
    gui().menuArrDown.setStyleImageRecolor(lv_color_hex(accent), LV_PART_MAIN);
    gui().menuArrDown.setStyleImageRecolorOpa(LV_OPA_COVER, LV_PART_MAIN);

    if(editing){
        gui().pressHint.setText("PRESS: DONE");
    }
    else if(mMenu.selectedRowIsToggle()){
        gui().pressHint.setText("PRESS: TOGGLE");
    }
    else if(mMenu.selectedRowIsAction()){
        gui().pressHint.setText("PRESS: OPEN");
    }
    else {
        gui().pressHint.setText("PRESS: EDIT");
    }
}

void MainScreen::onStart(){
    // nothing to do
}

void MainScreen::onBecomeEnabled(){
    mMenuTimer.start(750_ms);
    mAxisTimer.start(2500_ms);
}

void MainScreen::onBecomeDisabled(){
    // nothing to do
}

void MainScreen::onUserInputTick(){
    // nothing to do
}

void MainScreen::onButtonEvent(const ButtonEvent& event){
    if(event->newState == ButtonState::PressedLong){
        host()->requestChangeScreen(ScreenId::Simple);
    }
    else if(event->newState == ButtonState::Released && event->oldState == ButtonState::Pressed){
        switch(mMenu.onButtonInput()){
        case MenuRowIndex::Diagnostics: host()->requestChangeScreen(ScreenId::Debug); break;
        case MenuRowIndex::EventLog: host()->requestChangeScreen(ScreenId::EventLog); break;
        case MenuRowIndex::StateLog: host()->requestChangeScreen(ScreenId::StateLog); break;
        case MenuRowIndex::Home: host()->requestChangeScreen(ScreenId::Simple); break;
        default: break;
        }
        markDirty(RenderFn::Menu);
    }
}

void MainScreen::onEncoderStepEvent(const EncoderStepEvent& event){
    if(event->steps != 0){
        mMenu.onEncoderInput(event->steps);
        markDirty(RenderFn::Menu);
    }
}

void MainScreen::handleMenuValueChanged(MenuRowIndex index, int32_t oldValue, int32_t newValue){
    switch(index){
        case MenuRowIndex::Power: {
            RequestFermentationModeEvent event = makeRequestFermentationModeEvent();
            event->enabled = newValue != 0;
            sendEvent(event);
            break;
        }  
        case MenuRowIndex::Target: {
            RequestTargetTemperatureEvent event = makeRequestTargetTemperatureEvent();
            event->targetTemperature = newValue*0.1f;
            sendEvent(event);
            break;
        }  
        case MenuRowIndex::FanMode: {
            RequestFanModeEvent event = makeRequestFanModeEvent();
            event->autoFan = newValue != 0;
            event->manualSpeed = mMenu.getFanManualSpeed();
            sendEvent(event);
            break;
        }
        case MenuRowIndex::FanSpeed: {
            RequestFanModeEvent event = makeRequestFanModeEvent();
            event->autoFan = mMenu.isFanAuto();
            event->manualSpeed = static_cast<float>(newValue) * 0.01f;
            sendEvent(event);
            break;
        }
        case MenuRowIndex::Brightness: {
            DisplayCommandEvent event = makeDisplayCommandEvent();
            event->brightness = static_cast<float>(newValue) * 0.01f;
            sendEvent(event);
            break;
        }
        case MenuRowIndex::History:
            setHistoryIndex(static_cast<HistoryIndex>(newValue));
            break;
        case MenuRowIndex::PlotMinimum:
            setTemperaturePlotRange(newValue, mTemperaturePlotMaximumCelsius);
            break;
        case MenuRowIndex::PlotMaximum:
            setTemperaturePlotRange(mTemperaturePlotMinimumCelsius, newValue);
            break;
        default: break;
    }
}

void MainScreen::onRender(){
    if(!isMarkedDirty(RenderFn::TimeSeries)){
        mTempChart.updateAll();
        mPowerChart.updateAll();
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
    const float power = voltage * current;
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
        gui().deltaInfo.value.setText(resovleSensorText());
        return;
    }
    float temperature = states().temperatureSample.getTemperatureCelcius();
    gui().tempGraph.value.setTextFormatted("%.1f°C", temperature);
    const float error = temperature - states().fermentationStatus.getTargetTemperature();
    gui().deltaInfo.value.setTextFormatted("%+.1f°", error);
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

void MainScreen::onRenderTimeAxis(){
    static const char* t1_arr[] = {"15s", "1m", "4m",  "15m", "1h", "3h",  "6h",  "1d"};
    static const char* t2_arr[] = {"30s", "2m", "8m",  "30m", "2h", "4h",  "12h", "2d"};
    static const char* t3_arr[] = {"45s", "3m", "12m", "45m", "3h", "9h",  "18h", "3d"};
    static const char* t4_arr[] = {"60s", "4m", "16m", "60m", "4h", "12h", "24h", "4d"};
    static const char* window_arr[] = {"1 MIN", "4 MIN", "16 MIN", "1 HOUR", "4 HOURS", "12 HOURS", "1 DAY", "4 DAYS"};
    gui().t1.setText(t1_arr[static_cast<uint8_t>(mHistoryIndex)]);
    gui().t2.setText(t2_arr[static_cast<uint8_t>(mHistoryIndex)]);
    gui().t3.setText(t3_arr[static_cast<uint8_t>(mHistoryIndex)]);
    gui().t4.setText(t4_arr[static_cast<uint8_t>(mHistoryIndex)]);
    gui().windowLabel.setText(window_arr[static_cast<uint8_t>(mHistoryIndex)]);
}

void MainScreen::onRenderTimeSeries(){
    GarboxHistory& history = GarboxHistory::Instance();
    mTempChart.attach(0, history.getMeasuredTemperatureSeries(mHistoryIndex));
    mTempChart.attach(1, history.getTargetTemperatureSeries(mHistoryIndex));
    mPowerChart.attach(0, history.getPowerSeries(mHistoryIndex));
}

void MainScreen::onRenderMenu(){
    mMenu.setPowerEnabled(states().fermentationStatus.getPowerOn());
    mMenu.setTargetTemperatureCelsius(states().fermentationStatus.getTargetTemperature());
    mMenu.setHistoryIndex(mHistoryIndex);
    mMenu.setPlotRange(mTemperaturePlotMinimumCelsius, mTemperaturePlotMaximumCelsius);
    mMenu.setFanAuto(states().fermentationStatus.getFanAuto());
    mMenu.setFanManualSpeed(states().fermentationStatus.getFanManualSpeed());
    mMenu.setBrightness(states().displayStatus.getBrightness());
    mMenu.render();
    updateMenuChrome();
}

void MainScreen::onDisplayStatusStateChanged(const DisplayStatusState& state){
    markDirty(RenderFn::Menu);
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
    markDirty(RenderFn::MeasuredTemperatureLabel);
    markDirty(RenderFn::Menu);
}

void MainScreen::setHistoryIndex(HistoryIndex index){
    mHistoryIndex = index;
    markDirty(RenderFn::TimeAxis);
    markDirty(RenderFn::TimeSeries);
    markDirty(RenderFn::Menu);
}

void MainScreen::setTemperaturePlotRange(int32_t minimumCelsius, int32_t maximumCelsius){
    if(minimumCelsius >= maximumCelsius){
        return;
    }

    mTemperaturePlotMinimumCelsius = minimumCelsius;
    mTemperaturePlotMaximumCelsius = maximumCelsius;

    const int32_t rangeCelsius = maximumCelsius - minimumCelsius;
    const int32_t paddingScaled = std::max(1, rangeCelsius * TempChartYScale / 20);
    const int32_t chartMinimum = minimumCelsius * TempChartYScale - paddingScaled;
    const int32_t chartMaximum = maximumCelsius * TempChartYScale + paddingScaled;
    const int32_t middleCelsius = (minimumCelsius + maximumCelsius) / 2;

    mTempChart.getLvChart().setAxisRange(LV_CHART_AXIS_PRIMARY_Y, chartMinimum, chartMaximum);

    const int32_t ticks[] = {minimumCelsius, middleCelsius, maximumCelsius};
    for(uint32_t i = 0; i < 3; i++){
        mTempGrid.setYTickPosition(
            i,
            MathUtils::Normalize(ticks[i] * TempChartYScale, chartMinimum, chartMaximum)
        );
        std::snprintf(mTemperatureGridLabels[i], sizeof(mTemperatureGridLabels[i]), "%d°C", ticks[i]);
        mTempGrid.setYTickLabel(i, mTemperatureGridLabels[i]);
    }

    mTempChart.getLvChart().refresh();
    markDirty(RenderFn::Menu);
}

} // namespace Garbox
