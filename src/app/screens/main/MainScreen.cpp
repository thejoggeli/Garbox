#include "MainScreen.h"

#include "core/log/Log.h"
#include "core/lvgl/LvglHelpers.h"
#include "core/util/function/default/MathFunctions.h"
#include <math.h>

namespace Garbox {


static constexpr uint32_t ColorRed = 0xB85450;
static constexpr uint32_t ColorBlue = 0x8AB4F4;
static constexpr uint32_t ColorGreen = 0x7BBF56;

static constexpr uint32_t GridHorizontalCount = 3;
static constexpr uint32_t GridVerticalCount = 5;
static constexpr uint32_t GridLineColor = 0x333333;
static constexpr uint32_t GridLineWidth = 2;

static constexpr uint32_t ChartsPointCount = 8*8+1;

static constexpr int32_t TempChartYMin = 15;
static constexpr int32_t TempChartYMax = 45;
static constexpr int32_t TempChartYRange = TempChartYMax - TempChartYMin;

static constexpr int32_t PowerChartYMin = -10;
static constexpr int32_t PowerChartYMax = 110;
static constexpr int32_t PowerChartYRange = PowerChartYMax - PowerChartYMin;

MainScreen::MainScreen() : 
    MainScreenAbs(),
    mTempLabel(LvglHelpers::createRotatedLabelRgb565(gui().tempGraph.labelContainer, {
        .text = "Temperature",
        .font = &lv_font_montserrat_14,
        .color = ColorBlue,
        .bgColor = 0x0,
        .angle = LvglHelpers::Angle::Deg270,
    })),
    mPowerLabel(LvglHelpers::createRotatedLabelRgb565(gui().powerGraph.labelContainer, {
        .text = "Power",
        .font = &lv_font_montserrat_14,
        .color = ColorRed,
        .bgColor = 0x0,
        .angle = LvglHelpers::Angle::Deg270,
    })),
    mTempGrid(
        gui().tempGraph.chart,
        GridHorizontalCount,
        GridVerticalCount,
        lv_color_hex(GridLineColor),
        GridLineWidth
    ),
    mPowerGrid(
        gui().tempGraph.chart,
        GridHorizontalCount,
        GridVerticalCount,
        lv_color_hex(GridLineColor),
        GridLineWidth
    ){}

void MainScreen::onInit(){
    initTemperatureChart();
    initPowerChart();

    const int32_t xMin = 5;
    const int32_t xMax = ChartsPointCount - 5;
    const int32_t xRange = xMax - xMin;

    // temperature horizontal grid
    mTempGrid.setHorizontalPosition(0, (1000 * (20 - TempChartYMin)) / TempChartYRange);
    mTempGrid.setHorizontalPosition(0, (1000 * (30 - TempChartYMin)) / TempChartYRange);
    mTempGrid.setHorizontalPosition(0, (1000 * (40 - TempChartYMin)) / TempChartYRange);

    // power horizontal grid
    mPowerGrid.setHorizontalPosition(0, (1000 * (20 - PowerChartYMin)) / PowerChartYRange);
    mPowerGrid.setHorizontalPosition(0, (1000 * (30 - PowerChartYMin)) / PowerChartYRange);
    mPowerGrid.setHorizontalPosition(0, (1000 * (40 - PowerChartYMin)) / PowerChartYRange);

    // charts vertical grid
    for(int32_t i = 0; i < GridVerticalCount; i++){
        int32_t tVal = (xRange*i)/(GridVerticalCount-1);
        int32_t permille = (1000 * xMin + tVal) / ChartsPointCount;
        mTempGrid.setVerticalPosition(0, permille);
        mPowerGrid.setVerticalPosition(0, permille);
    }
}

void MainScreen::initTemperatureChart(){

    // setup temperature chart
    LvChart& chart = gui().tempGraph.chart;
    chart.setType(LV_CHART_TYPE_LINE);
    chart.setPointCount(ChartsPointCount);
    chart.setAxisRange(LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    chart.setDivLineCount(5, 5);
    chart.setUpdateMode(LV_CHART_UPDATE_MODE_SHIFT);
    chart.setStyleSize(0, 0, LV_PART_INDICATOR); // markers size
    chart.setStyleLineWidth(3, LV_PART_ITEMS); // series line width

    // create temperature series
    LvChartSeries* series = chart.addSeries(lv_color_hex(ColorBlue)); 
    chart.hideSeries(series, false);

    // fill temperature values
    const MathFunctionIfc& mathFn = MathFunctions::GetSinAnim(); 
    for (int i = 0; i < ChartsPointCount; i++) {
        const float step = 1.0f/(8*8);
        const float x = fmodf(static_cast<float>(i) * step, 1.0f);
        const float value = mathFn.evaluate(x) * 75.0f + 12.5f;
        chart.setNextValue(series, value);
    }

    chart.refresh();
    mTempSeries = series;
}

void MainScreen::initPowerChart(){

    // setup temperature chart
    LvChart& chart = gui().tempGraph.chart;
    chart.setType(LV_CHART_TYPE_LINE);
    chart.setPointCount(ChartsPointCount);
    chart.setAxisRange(LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    chart.setDivLineCount(5, 5);
    chart.setUpdateMode(LV_CHART_UPDATE_MODE_SHIFT);
    chart.setStyleSize(0, 0, LV_PART_INDICATOR); // markers size
    chart.setStyleLineWidth(3, LV_PART_ITEMS); // series line width

    // create temperature series
    LvChartSeries* series = chart.addSeries(lv_color_hex(ColorBlue)); 
    chart.hideSeries(series, false);

    // fill temperature values
    const MathFunctionIfc& mathFn = MathFunctions::GetGamma22(); 
    for (int i = 0; i < ChartsPointCount; i++) {
        const float step = 1.0f/(8*8);
        const float x = fmodf(static_cast<float>(i) * step, 1.0f);
        const float value = mathFn.evaluate(x) * 75.0f + 12.5f;
        chart.setNextValue(series, value);
    }

    chart.refresh();
    mPowerSeries = series;
}

void MainScreen::onStart(){
    // nothing to do
}

void MainScreen::onBecomeEnabled(){
    // nothing to do
}

void MainScreen::onBecomeDisabled(){
    // nothing to do
}

void MainScreen::onUpdateScreen(){
    const MathFunctionIfc& mathFn = MathFunctions::GetSinAnim(); 
    const float step = 1.0f/(8*8);
    static float t = step;
    LvChart& chart = gui().tempGraph.chart;
    const float x = fmodf(t, 1.0f);
    const float value = mathFn.evaluate(x) * 75.0f + 12.5f;
    chart.setNextValue(mTempSeries, value);
    t += step;
    if(t >= 0.99f){
        t = 0.0f;
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
    // HeatpadState state = model().getHeatpadState();
    // if(state == HeatpadState::Disabled){
    //     gui().statusHeat.value.setText("Off");
    //     // gui().statusHeat.unit.setHidden(true);
    // }
    // else {
    //     gui().statusHeat.value.setTextFormatted("%.1f", model().getHeatpadCurrentDuty()*100.0f);
    //     // gui().statusHeat.unit.setHidden(false);
    // }
}

void MainScreen::onApplyHeatpadMeasure(){
    const float voltage = model().getHeatpadMeasuredVoltage();
    const float current = model().getHeatpadMeasuredCurrent();
    const float duty = model().getHeatpadCurrentDuty();
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
    // if(isSensorOk()){
    //     gui().sensorTemperature.value.setTextFormatted("%.1f", model().getMeasuredTemperature());
    //     // gui().sensorTemperature.unit.setHidden(false);
    // }
    // else {
    //     gui().sensorTemperature.value.setText(resovleSensorText());
    //     // gui().sensorTemperature.unit.setHidden(true);
    // }
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