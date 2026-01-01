#include "SimpleScreen.h"

#include <math.h>
#include "app/providers/ColorMaps.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

static constexpr uint32_t ColorRed = 0xB85450;
static constexpr uint32_t ColorGreen = 0x7BBF56;

static constexpr float ArcTempMin = 15.0f;
static constexpr float ArcTempMax = 50.0f;
static constexpr uint16_t ArcAngleMin = 0;
static constexpr uint16_t ArcAngleMax = 359;

SimpleScreen::SimpleScreen() : SimpleScreenAbs(){
    // to be implemented
}

void SimpleScreen::onInit(){
    gui().tempLabel.setFont(&lv_font_montserrat_12);
    gui().tempValue.setFont(&lv_font_montserrat_30);
    gui().tempUnit.setFont(&lv_font_montserrat_14);
    gui().infoTiles.setFont(&lv_font_montserrat_12);

    initArc(gui().arcBg);
    initArc(gui().arcMeasured);
    initArc(gui().arcMeasuredEnd);
    initArc(gui().arcMeasuredBorder);
    initArc(gui().arcTarget);
    initArc(gui().arcTargetEnd);
    initArc(gui().arcTargetBorder);
}

void SimpleScreen::initArc(LvArc& arc){
    arc.removeKnob();
    arc.setClickable(false);
    arc.setArcRotation(90);
}

void SimpleScreen::setArcTemperature(ArcIndex index, float temperature){
    
    // map temperature to arc angle
    constexpr float tMin = ArcTempMin;
    constexpr float tMax = ArcTempMax;
    constexpr float aMin = ArcAngleMin;
    constexpr float aMax = ArcAngleMax;
    const float angleFloat = MathUtils::Map(temperature, tMin, tMax, aMin, aMax);
    const float angleClamped = MathUtils::Clamp(angleFloat, aMin, aMax);
    const uint16_t angle = static_cast<uint16_t>(angleClamped+0.5f);
    
    // set final arc angle
    switch(index){
        case ArcIndex::MeasuredTemperature:
            gui().arcMeasured.setArcEndAngle(angle);
            gui().arcMeasuredEnd.setArcAngles(angle, angle+1);
            gui().arcMeasuredBorder.setArcEndAngle(angle);
            break;
        case ArcIndex::TargetTemperature:
            gui().arcTarget.setArcEndAngle(angle);
            gui().arcTargetEnd.setArcAngles(angle, angle+1);
            gui().arcTargetBorder.setArcEndAngle(angle);
            break;
        default: TriggerExit("SimpleScreen", "unhandled arc index");
    }
}

void SimpleScreen::updateArcColor(){
    const ColorMap& cm = ColorMaps::GetCoolWarm();
    const float tempMeasured = states().fermentationStatus.getTargetTemperature(); 
    const float tempTarget = states().temperatureSample.getTemperatureCelcius();
    const float range = 5.0f;
    const float t = (tempMeasured - tempTarget) / range + 0.5f;
    LabColor lab = cm.interpolateLab(t);
    uint32_t hex = lab.hexStandard();
    gui().arcMeasured.setArcColor(lv_color_hex(hex));
}

void SimpleScreen::onStart(){
    // to be implemented
}

void SimpleScreen::onBecomeEnabled(){
    // to be implemented
}

void SimpleScreen::onBecomeDisabled(){
    // to be implemented
}

void SimpleScreen::onButtonEvent(const ButtonEvent& event){
    if(event->newState == ButtonState::Pressed){
        RequestFermentationModeEvent request = makeRequestFermentationModeEvent();
        request->enabled = !states().fermentationStatus.getPowerOn();
        sendEvent(request);
    }
}

void SimpleScreen::onButtonRepeatEvent(const ButtonRepeatEvent& event){
    if(event->count > 5){
        host()->requestChangeScreen(ScreenId::Main);
    }
}

void SimpleScreen::onEncoderStepEvent(const EncoderStepEvent& event){

    // compute new target temperature 
    constexpr float scaleFactor = 10.0f;
    const int16_t tempOldInt = states().fermentationStatus.getTargetTemperature() * scaleFactor + 0.5f;
    const int16_t tempNewInt = tempOldInt + event->steps;
    const float tempNew = static_cast<float>(tempNewInt) / scaleFactor;
    const float tempNewClamped = MathUtils::Clamp(tempNew, ArcTempMin, ArcTempMax);

    // send target temperature request
    RequestTargetTemperatureEvent request = makeRequestTargetTemperatureEvent();
    request->targetTemperature = tempNewClamped;
    sendEvent(request);
}

void SimpleScreen::onRender(){
    // nothing to do
}

void SimpleScreen::onRenderEngineStatus(){
    const FermentationStatusState& status = states().fermentationStatus;
    if(status.getPowerOn()){
        gui().fermentationStatus.value.setText("On");
        gui().fermentationStatus.body.setBorderColor(lv_color_hex(ColorGreen));
        gui().fermentationStatus.body.setBgColor(lv_color_hex(ColorGreen));
    }
    else {
        gui().fermentationStatus.value.setText("Off");
        gui().fermentationStatus.body.setBorderColor(lv_color_hex(ColorRed));
        gui().fermentationStatus.body.setBgColor(lv_color_hex(ColorRed));
    }
}

void SimpleScreen::onRenderTargetTemperature(){
    const float targetTemperature = states().fermentationStatus.getTargetTemperature();
    gui().tempValue.setTextFormatted("%.1f", targetTemperature);
    setArcTemperature(ArcIndex::TargetTemperature, targetTemperature);
}

void SimpleScreen::onRenderMeasuredTemperature(){
    const float measuredTemperature = states().temperatureSample.getTemperatureCelcius();
    gui().measuredTemperature.value.setTextFormatted("%.1f°C", measuredTemperature);
    setArcTemperature(ArcIndex::MeasuredTemperature, measuredTemperature);
    updateArcColor();
}

void SimpleScreen::onRenderMeasuredHumidity(){
    const float measuredHumidity = states().temperatureSample.getHumidityRelative();
    gui().measuredHumidity.value.setTextFormatted("%.1f%%", measuredHumidity);
}

void SimpleScreen::onRenderHeaterPower(){
    const float heaterPowerPercent = states().heatpadStatus.getNextDutyCycle() * 100.0f;
    gui().heaterPower.value.setTextFormatted("%.1f%%", heaterPowerPercent);
}

void SimpleScreen::onRenderFanStatus(){
    // nothing to do
}

void SimpleScreen::onFanStatusStateChanged(const FanStatusState& state){
    markDirty(RenderFn::FanStatus);
}

void SimpleScreen::onFanSampleStateChanged(const FanSampleState& state){
    markDirty(RenderFn::FanStatus);
}

void SimpleScreen::onHeatpadStatusStateChanged(const HeatpadStatusState& state){
    markDirty(RenderFn::HeaterPower);
}

void SimpleScreen::onHeatpadSampleStateChanged(const HeatpadSampleState& state){
    markDirty(RenderFn::HeaterPower);
}

void SimpleScreen::onTemperatureStatusStateChanged(const TemperatureStatusState& state){
    markDirty(RenderFn::MeasuredTemperature);
    markDirty(RenderFn::MeasuredHumidity);
}

void SimpleScreen::onTemperatureSampleStateChanged(const TemperatureSampleState& state){
    markDirty(RenderFn::MeasuredTemperature);
    markDirty(RenderFn::MeasuredHumidity);
}

void SimpleScreen::onFermentationStatusStateChanged(const FermentationStatusState& state){
    markDirty(RenderFn::EngineStatus);
    markDirty(RenderFn::FanStatus);
    markDirty(RenderFn::TargetTemperature);
    markDirty(RenderFn::MeasuredTemperature);
}

} // namespace Garbox