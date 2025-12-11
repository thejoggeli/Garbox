#include "MainScreen.h"

#include "core/log/Log.h"
#include "core/lvgl/LvglHelpers.h"

namespace Garbox {

static const uint32_t ColorRed = 0xB85450;
static const uint32_t ColorBlue = 0x8AB4F4;
static const uint32_t ColorGreen = 0x7BBF56;

MainScreen::MainScreen() : 
    MainScreenAbs(),
    mTemperatureLabel(LvglHelpers::createRotatedLabelRgb565(gui().tempGraph.labelContainer, {
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
    })){}

void MainScreen::onInit(){
    // setBackgroundColor(0x0);
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
    // nothing to do
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