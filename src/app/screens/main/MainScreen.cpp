#include "MainScreen.h"

#include "core/log/Log.h"

namespace Garbox {

MainScreen::MainScreen() : MainScreenAbs(){}

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
        case HeaterEngineState::Null:
        case HeaterEngineState::Count:
        case HeaterEngineState::InvalidInput:
            return "Error";
        case HeaterEngineState::Reset:
        case HeaterEngineState::Cooldown:
            return "Off";
        case HeaterEngineState::Ready:
        case HeaterEngineState::Regulating:
            return "On";
        case HeaterEngineState::OverTemperature:
            return "Overheat";
        default:
            return "Invalid";
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
        gui().sensorFan.value.setText("Stall");
        // gui().sensorFan.unit.setHidden(true);
    }
    else {
        uint32_t rpm = model().getFanMeasuredRpm();
        gui().sensorFan.value.setTextFormatted("%u", rpm);
        // gui().sensorFan.unit.setHidden(false);
    }
}

void MainScreen::onApplyFanTargetSpeed(){
    FanState state = model().getFanState();
    if(state == FanState::Disabled){
        gui().statusFan.value.setText("Off");
        // gui().statusFan.unit.setHidden(true);
    }
    else {
        float targetSpeed = model().getFanTargetSpeed() * 100.0f;
        gui().statusFan.value.setTextFormatted("%.1f", targetSpeed);
        // gui().statusFan.unit.setHidden(false);
    }
}

void MainScreen::onApplyHeatpadStatus(){
    HeatpadState state = model().getHeatpadState();
    if(state == HeatpadState::Disabled){
        gui().statusHeat.value.setText("Off");
        // gui().statusHeat.unit.setHidden(true);
    }
    else {
        gui().statusHeat.value.setTextFormatted("%.1f", model().getHeatpadCurrentDuty()*100.0f);
        // gui().statusHeat.unit.setHidden(false);
    }
}

void MainScreen::onApplyHeatpadMeasure(){
    const float voltage = model().getHeatpadMeasuredVoltage();
    const float current = model().getHeatpadMeasuredCurrent();
    const float duty = model().getHeatpadCurrentDuty();
    const float power = voltage * current * duty;
    gui().sensorPower.value.setTextFormatted("%.1f", power);
}

void MainScreen::onApplyDisplayBrightness(){
    float brightness = model().getDisplayBrightness() * 100.0f;
    gui().settingBrightness.value.setTextFormatted("%.1f%%", brightness);
}

void MainScreen::onApplySensorStatus(){
    // everything handled in temperature and humidity methods
}

void MainScreen::onApplyMeasuredTemperature(){
    if(isSensorOk()){
        gui().sensorTemperature.value.setTextFormatted("%.1f", model().getMeasuredTemperature());
        // gui().sensorTemperature.unit.setHidden(false);
    }
    else {
        gui().sensorTemperature.value.setText(resovleSensorText());
        // gui().sensorTemperature.unit.setHidden(true);
    }
}

void MainScreen::onApplyMeasuredHumidity(){
    if(isSensorOk()){
        gui().sensorHumidity.value.setTextFormatted("%.1f", model().getMeasuredHumidity());
        // gui().sensorHumidity.unit.setHidden(false);
    }
    else {
        gui().sensorHumidity.value.setText(resovleSensorText());
        // gui().sensorHumidity.unit.setHidden(true);
    }
}

void MainScreen::onApplyTargetTemperature(){
    HeaterEngineState state = model().getEngineState();
    switch(state){
        case HeaterEngineState::Ready:
        case HeaterEngineState::Regulating:
        case HeaterEngineState::OverTemperature: {
            gui().statusTemperature.value.setTextFormatted("%.1f", model().getTargetTemperature());
            // gui().statusTemperature.unit.setHidden(false);            
            break;
        }
        default: {
            gui().statusTemperature.value.setText("Off");
            // gui().statusTemperature.unit.setHidden(true);   
            break;   
        }
    }
}

void MainScreen::onApplyEngineState(){
    gui().statusEngine.value.setText(resovleEngineStateText());
}

} // namespace Garbox