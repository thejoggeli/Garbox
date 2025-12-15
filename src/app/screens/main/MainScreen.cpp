#include "MainScreen.h"

#include "core/log/Log.h"
#include "core/lvgl/helpers/RotationRenderer.h"
#include "core/util/function/default/MathFunctions.h"
#include "core/util/math/MathUtils.h"
#include <math.h>

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

static constexpr uint32_t ChartsPointCount = 8*8+1;

static constexpr int32_t TempChartYScale = 1024*8; // scale y-values by factor to prevent staircase effect (LVGL chart uses ints internally)
static constexpr int32_t TempChartYMin = TempChartYScale * 16;
static constexpr int32_t TempChartYMax = TempChartYScale * 44;
 
static constexpr int32_t PowerChartYScale = 1024*8; // scale y-values factor to prevent staircase effect (LVGL chart uses ints internally)
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
    mPowerTimer.start(250_ms);
}

void MainScreen::onBecomeEnabled(){
    // nothing to do
}

void MainScreen::onBecomeDisabled(){
    // nothing to do
}

void MainScreen::onUpdateScreen(){

    // update power chart
    if(mPowerTimer.isExpired()){
        mPowerTimer.restart();
        const float power = model().getHeatpadNextDuty() * 100.0f;
        gui().powerGraph.chart.setNextValue(mPowerSeries, power * PowerChartYScale);
    }
}

void MainScreen::onDisplayCommand(const DisplayCommandEvent& event){
    // nothing to do
}

void MainScreen::onFanCommand(const FanCommandEvent& event){
    // nothing to do
}

void MainScreen::onHeatpadCommand(const HeatpadCommandEvent& event){
    // nothing to do
}

bool MainScreen::isSensorOk(){
    return model().getShtPowerEnabled() && !model().getShtResetting() && model().getShtDriverEnabled() && model().getShtHasSample();
}

const char* MainScreen::resovleEngineStateText(){
    const HeaterEngineState state = model().getEngineState();

    switch(state){
        case HeaterEngineState::Reset:
            return "Reset";
        case HeaterEngineState::Cooldown:
            return "Off";
        case HeaterEngineState::Ready:
            return "Ready";
        case HeaterEngineState::Regulating:
            return "Running";
        case HeaterEngineState::OverTemperature:
            return "Overheat";
        case HeaterEngineState::InvalidInput:
            return "Error #2";
        case HeaterEngineState::Null:
            return "Error #0";
        case HeaterEngineState::Count:
        default:
            return "Error #1";
    }
}

uint32_t MainScreen::resovleEngineStateColor(){
    const HeaterEngineState state = model().getEngineState();

    switch(state){
        case HeaterEngineState::Cooldown:
            return ColorBlue; // blue
        case HeaterEngineState::Reset:
        case HeaterEngineState::Ready:
        case HeaterEngineState::Regulating:
            return ColorGreen; // green
        case HeaterEngineState::OverTemperature:
        case HeaterEngineState::InvalidInput:
        case HeaterEngineState::Null:
        case HeaterEngineState::Count:
        default:
            return ColorRed; // red
    }
}

const char* MainScreen::resovleSensorText(){
    if(model().getShtResetting()){
        return "Reset";
    }
    else if(!model().getShtPowerEnabled() || !model().getShtDriverEnabled()){
        return "Off";
    }
    else if(!model().getShtHasSample()){
        return "Busy";
    }
    return "Ok";
}

void MainScreen::onApplyFanStatus(){
    FanState state = model().getFanState();
    if(state == FanState::Stalled){
        gui().fanInfo.value.setText("Stall");
        // gui().sensorFan.unit.setHidden(true);
    }
    else {
        float rpm = model().getFanMeasuredRpm();
        uint32_t speed = std::clamp((rpm / 2250.0f) * 100.0f, 0.0f, 100.0f) + 0.5f; 
        gui().fanInfo.value.setTextFormatted("%u%%", speed);
        // gui().sensorFan.unit.setHidden(false);
    }
}

void MainScreen::onApplyFanTargetSpeed(){
    // FanState state = model().getFanState();
    // if(state == FanState::Disabled){
    //     gui().fanInfo.value.setText("Off");
    //     // gui().statusFan.unit.setHidden(true);
    // }
    // else {
    //     float targetSpeed = model().getFanTargetSpeed() * 100.0f;
    //     gui().fanInfo.value.setTextFormatted("%.1f", targetSpeed);
    //     // gui().statusFan.unit.setHidden(false);
    // }
}

void MainScreen::onApplyHeatpadStatus(){
    HeatpadState state = model().getHeatpadState();
    if(state == HeatpadState::Disabled){
        gui().powerGraph.value.setText("Off");
        return;
    }
    const float power = model().getHeatpadNextDuty() * 100.0f;
    gui().powerGraph.value.setTextFormatted("%.1f%%", power);
}

void MainScreen::onApplyHeatpadMeasure(){
    const float voltage = model().getHeatpadMeasuredVoltage();
    const float current = model().getHeatpadMeasuredCurrent();
    const float duty = model().getHeatpadNextDuty();
    const float power = voltage * current * duty;
    gui().powerInfo.value.setTextFormatted("%.1fW", power);
}

void MainScreen::onApplyDisplayBrightness(){
    // float brightness = model().getDisplayBrightness() * 100.0f;
    // gui().settingBrightness.value.setTextFormatted("%.1f%%", brightness);
}

void MainScreen::onApplySensorStatus(){
    // everything handled in temperature and humidity methods
}

void MainScreen::onApplyMeasuredTemperature(){

    if(!isSensorOk()){
        gui().tempGraph.value.setTextFormatted(resovleSensorText());
        return;
    }

    // set chart temperature
    float temperature = model().getMeasuredTemperature();
    gui().tempGraph.chart.setNextValue(mTempSeries, temperature * TempChartYScale);
    gui().tempGraph.value.setTextFormatted("%.1f°C", temperature);

    // set chart target temperature
    float temperatureTarget = model().getTargetTemperature();
    gui().tempGraph.chart.setNextValue(mTempTargetSeries, temperatureTarget * TempChartYScale);
}

void MainScreen::onApplyMeasuredHumidity(){
    if(isSensorOk()){
        gui().humidInfo.value.setTextFormatted("%.1f%%", model().getMeasuredHumidity());
        // gui().sensorHumidity.unit.setHidden(false);
    }
    else {
        gui().humidInfo.value.setText(resovleSensorText());
        // gui().sensorHumidity.unit.setHidden(true);
    }
}

void MainScreen::onApplyTargetTemperature(){
    // HeaterEngineState state = model().getEngineState();
    // switch(state){
    //     case HeaterEngineState::Ready:
    //     case HeaterEngineState::Regulating:
    //     case HeaterEngineState::OverTemperature: {
    //         gui().statusTemperature.value.setTextFormatted("%.1f", model().getTargetTemperature());
    //         // gui().statusTemperature.unit.setHidden(false);            
    //         break;
    //     }
    //     default: {
    //         gui().statusTemperature.value.setText("Off");
    //         // gui().statusTemperature.unit.setHidden(true);   
    //         break;   
    //     }
    // }
}

void MainScreen::onApplyEngineState(){
    gui().systemState.setText(resovleEngineStateText());
    gui().systemStateBg.setBgColor(lv_color_hex(resovleEngineStateColor()));
}

} // namespace Garbox