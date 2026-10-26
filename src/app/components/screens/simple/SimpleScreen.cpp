#include "SimpleScreen.h"

#include <math.h>
#include "app/components/screens/ScreenCommandBar.h"
#include "app/providers/ColorMaps.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

static constexpr uint32_t ColorRed = 0xB85450;
static constexpr uint32_t ColorGreen = 0x7BBF56;
static constexpr uint32_t ColorBlue = 0x5B8FC9;
static constexpr uint32_t ColorAmber = 0xD1B254;
static constexpr uint32_t ColorMuted = 0x60717B;

static constexpr float ArcTempMin = 15.0f;
static constexpr float ArcTempMax = 50.0f;
static constexpr uint16_t ArcAngleMin = 0;
static constexpr uint16_t ArcAngleMax = 359;

SimpleScreen::SimpleScreen() : SimpleScreenAbs(){
    // to be implemented
}

void SimpleScreen::onInit(){
    gui().statusText.setFont(&lv_font_montserrat_12);
    gui().sensorStatus.setFont(&lv_font_unscii_8);
    gui().fanModeLabel.setFont(&lv_font_unscii_8);
    gui().measuredLabel.setFont(&lv_font_montserrat_10);
    gui().measuredValue.setFont(&lv_font_montserrat_36);
    gui().measuredUnit.setFont(&lv_font_montserrat_20);
    gui().temperatureDelta.setFont(&lv_font_montserrat_10);
    gui().targetLabel.setFont(&lv_font_montserrat_10);
    gui().targetValue.setFont(&lv_font_montserrat_20);
    gui().targetUnit.setFont(&lv_font_montserrat_12);
    gui().targetHint.setFont(&lv_font_unscii_8);
    gui().humidity.label.setFont(&lv_font_unscii_8);
    gui().heater.label.setFont(&lv_font_unscii_8);
    gui().fan.label.setFont(&lv_font_unscii_8);
    gui().humidity.value.setFont(&lv_font_montserrat_12);
    gui().heater.value.setFont(&lv_font_montserrat_12);
    gui().fan.value.setFont(&lv_font_montserrat_12);
    gui().targetRotateUp.setStyleImageRecolor(lv_color_hex(ColorBlue), LV_PART_MAIN);
    gui().targetRotateUp.setStyleImageRecolorOpa(LV_OPA_COVER, LV_PART_MAIN);
    gui().targetRotateDown.setStyleImageRecolor(lv_color_hex(ColorBlue), LV_PART_MAIN);
    gui().targetRotateDown.setStyleImageRecolorOpa(LV_OPA_COVER, LV_PART_MAIN);

    // Metric cards are fixed status displays, not scrollable viewports. LVGL's
    // default scrollbar becomes visible when font metrics round up by a pixel,
    // which looks like an unexplained grey indicator at the card's right edge.
    auto configureMetricCard = [](SimpleScreenGui::MetricCard& card){
        card.body.setScrollable(false);
        lv_obj_set_scrollbar_mode(card.body.raw(), LV_SCROLLBAR_MODE_OFF);
        card._namelessContainer_0.setScrollable(false);
        lv_obj_set_scrollbar_mode(card._namelessContainer_0.raw(), LV_SCROLLBAR_MODE_OFF);
    };
    configureMetricCard(gui().humidity);
    configureMetricCard(gui().heater);
    configureMetricCard(gui().fan);

    ConfigureScreenCommandBar(
        gui().footer,
        gui().pressHint,
        gui().holdHint,
        "PRESS: START/STOP",
        "HOLD: MENU"
    );

    initArc(gui().arcBg);
    initArc(gui().arcMeasured);
    initArc(gui().arcTarget);
}

void SimpleScreen::initArc(LvArc& arc){
    arc.removeKnob();
    arc.setClickable(false);
    arc.setArcRotation(90);
}

bool SimpleScreen::isSensorOk(){
    const TemperatureStatusState& status = states().temperatureStatus;
    return status.getPowerEnabled()
        && status.getDriverEnabled()
        && !status.getResetting()
        && status.getHasFirstSample();
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
            break;
        case ArcIndex::TargetTemperature:
            gui().arcTarget.setArcEndAngle(angle);
            break;
        default: TriggerExit("SimpleScreen", "unhandled arc index");
    }
}

void SimpleScreen::updateArcColor(){
    const ColorMap& cm = ColorMaps::GetCoolWarm();
    const float tempMeasured = states().temperatureSample.getTemperatureCelcius();
    const float tempTarget = states().fermentationStatus.getTargetTemperature();
    const float range = 5.0f;
    const float t = (tempTarget - tempMeasured) / range + 0.5f;
    LabColor lab = cm.interpolateLab(t);
    uint32_t hex = lab.hexStandard();
    gui().arcMeasured.setArcColor(lv_color_hex(hex));
}

void SimpleScreen::onStart(){
    // to be implemented
}

void SimpleScreen::onBecomeEnabled(){
    mFermentationStatusGlow = false;
}

void SimpleScreen::onBecomeDisabled(){
    // to be implemented
}

void SimpleScreen::onButtonEvent(const ButtonEvent& event){
    if(event->newState == ButtonState::Pressed){
        // enable engine status glow
        mFermentationStatusGlow = true;
        markDirty(RenderFn::EngineStatus);
    }
    else if(event->newState == ButtonState::Released && event->oldState == ButtonState::Pressed){
        // disable engine status glow
        mFermentationStatusGlow = false;
        markDirty(RenderFn::EngineStatus);

        // send request toggle engine power 
        RequestFermentationModeEvent request = makeRequestFermentationModeEvent();
        request->enabled = !states().fermentationStatus.getPowerOn();
        sendEvent(request);
    }
    else if(event->newState == ButtonState::PressedLong){
        // send request change screen
        host()->requestChangeScreen(ScreenId::Main);
    }
}

void SimpleScreen::onButtonRepeatEvent(const ButtonRepeatEvent& event){
    // nothing to do
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
    const char* text = "OFF";
    uint32_t color = 0x3A2528;
    if(status.getPowerOn()){
        switch(status.getState()){
        case FermentationState::Ready: text = "READY"; color = ColorAmber; break;
        case FermentationState::Regulating:
            text = states().heatpadStatus.getNextDutyCycle() > 0.01f ? "HEATING" : "HOLDING";
            color = ColorGreen;
            break;
        case FermentationState::Cooldown: text = "COOLING"; color = ColorBlue; break;
        case FermentationState::OverTemperature: text = "TOO HOT"; color = ColorRed; break;
        case FermentationState::InvalidInput: text = "SENSOR"; color = ColorRed; break;
        default: text = "STARTING"; color = ColorAmber; break;
        }
    }
    gui().statusText.setText(text);
    if(mFermentationStatusGlow){
        gui().statusPill.setBgColor(lv_color_hex(0x52616A));
    }
    else {
        gui().statusPill.setBgColor(lv_color_hex(color));
    }
}

void SimpleScreen::onRenderTargetTemperature(){
    const float targetTemperature = states().fermentationStatus.getTargetTemperature();
    gui().targetValue.setTextFormatted("%.1f", targetTemperature);
    setArcTemperature(ArcIndex::TargetTemperature, targetTemperature);
}

void SimpleScreen::onRenderMeasuredTemperature(){
    const TemperatureStatusState& sensor = states().temperatureStatus;
    if(isSensorOk()){
        gui().sensorStatus.setText("SENSOR OK");
        gui().sensorStatus.setTextColor(lv_color_hex(ColorGreen));
    }
    else if(sensor.getResetting() || (sensor.getPowerEnabled() && sensor.getDriverEnabled())){
        gui().sensorStatus.setText("SENSOR WAIT");
        gui().sensorStatus.setTextColor(lv_color_hex(ColorAmber));
    }
    else if(!sensor.getPowerEnabled()){
        gui().sensorStatus.setText("SENSOR OFF");
        gui().sensorStatus.setTextColor(lv_color_hex(ColorMuted));
    }
    else {
        gui().sensorStatus.setText("SENSOR FAULT");
        gui().sensorStatus.setTextColor(lv_color_hex(ColorRed));
    }

    if(!isSensorOk()){
        gui().measuredValue.setText("--.-");
        gui().temperatureDelta.setText("WAITING FOR SENSOR");
        gui().temperatureDelta.setTextColor(lv_color_hex(ColorMuted));
        setArcTemperature(ArcIndex::MeasuredTemperature, ArcTempMin);
        return;
    }
    const float measuredTemperature = states().temperatureSample.getTemperatureCelcius();
    const float delta = measuredTemperature - states().fermentationStatus.getTargetTemperature();
    gui().measuredValue.setTextFormatted("%.1f", measuredTemperature);
    gui().temperatureDelta.setTextFormatted("TARGET %+.1f°C", delta);
    const uint32_t deltaColor = delta > 0.5f ? ColorRed : (delta < -0.5f ? ColorBlue : ColorGreen);
    gui().temperatureDelta.setTextColor(lv_color_hex(deltaColor));
    setArcTemperature(ArcIndex::MeasuredTemperature, measuredTemperature);
    updateArcColor();
}

void SimpleScreen::onRenderMeasuredHumidity(){
    if(!isSensorOk()){
        gui().humidity.value.setText("--.-%");
        return;
    }
    const float measuredHumidity = states().temperatureSample.getHumidityRelative();
    gui().humidity.value.setTextFormatted("%.1f%%", measuredHumidity);
}

void SimpleScreen::onRenderHeaterPower(){
    const float heaterPowerPercent = states().heatpadStatus.getNextDutyCycle() * 100.0f;
    gui().heater.value.setTextFormatted("%.0f%%", heaterPowerPercent);
}

void SimpleScreen::onRenderFanStatus(){
    gui().fanModeLabel.setText(
        states().fermentationStatus.getFanAuto() ? "AUTO FAN" : "MANUAL FAN"
    );
    if(states().fanStatus.getState() == FanState::Stalled){
        gui().fan.value.setText("STALLED");
        gui().fan.value.setTextColor(lv_color_hex(ColorRed));
        return;
    }
    const float rpm = states().fanSample.getMeasuredRpm();
    if(states().fanStatus.getTargetSpeed() <= 0.0f && rpm < 1.0f){
        gui().fan.value.setText("OFF");
    }
    else {
        gui().fan.value.setTextFormatted("%.0f RPM", rpm);
    }
    gui().fan.value.setTextColor(lv_color_hex(0xFFFFFF));
}

void SimpleScreen::onFanStatusStateChanged(const FanStatusState& state){
    markDirty(RenderFn::FanStatus);
}

void SimpleScreen::onFanSampleStateChanged(const FanSampleState& state){
    markDirty(RenderFn::FanStatus);
}

void SimpleScreen::onHeatpadStatusStateChanged(const HeatpadStatusState& state){
    markDirty(RenderFn::HeaterPower);
    markDirty(RenderFn::EngineStatus);
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
