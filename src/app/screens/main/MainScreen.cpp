#include "MainScreen.h"

#include "core/log/Log.h"

LV_IMAGE_DECLARE(heat16);
LV_IMAGE_DECLARE(fan16);
LV_IMAGE_DECLARE(temperature16);
LV_IMAGE_DECLARE(humidity16);
LV_IMAGE_DECLARE(power16);

namespace Garbox {

MainScreen::MainScreen() : 
    MainScreenAbs(),
    mHeatImage(mContainer),
    mFanImage(mContainer),
    mTemperatureImage(mContainer),
    mHumidityImage(mContainer),
    mPowerImage(mContainer),
    mHeatLabel(mContainer),
    mFanLabel(mContainer),
    mTemperatureLabel(mContainer),
    mHumidityLabel(mContainer),
    mPowerLabel(mContainer){}

void MainScreen::onInit(){
    setBackgroundColor(0x0);

    mHeatLabel.setFont(&lv_font_montserrat_14);
    mFanLabel.setFont(&lv_font_montserrat_14);
    mTemperatureLabel.setFont(&lv_font_montserrat_14);
    mHumidityLabel.setFont(&lv_font_montserrat_14);
    mPowerLabel.setFont(&lv_font_montserrat_14);

    mHeatLabel.setTextColor(lv_color_hex(0xFFFFFF));
    mFanLabel.setTextColor(lv_color_hex(0xFFFFFF));
    mTemperatureLabel.setTextColor(lv_color_hex(0xFFFFFF));
    mHumidityLabel.setTextColor(lv_color_hex(0xFFFFFF));
    mPowerLabel.setTextColor(lv_color_hex(0xFFFFFF));

    mHeatLabel.setPosition(48, 25);
    mFanLabel.setPosition(48, 25+24);
    mTemperatureLabel.setPosition(48, 25+24*2);
    mHumidityLabel.setPosition(48, 25+24*3);
    mPowerLabel.setPosition(48, 25+24*4);

    mHeatImage.setSource(&heat16);
    mFanImage.setSource(&fan16);
    mTemperatureImage.setSource(&temperature16);
    mHumidityImage.setSource(&humidity16);
    mPowerImage.setSource(&power16);

    mHeatImage.setPosition(24, 24);
    mFanImage.setPosition(24, 24+24);
    mTemperatureImage.setPosition(24, 24+24*2);
    mHumidityImage.setPosition(24, 24+24*3);
    mPowerImage.setPosition(24, 24+24*4);
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
    return mModel.getShtPowerEnabled() && !mModel.getShtResetting() && mModel.getShtDriverEnabled() && mModel.getShtHasSample();
}

const char* MainScreen::resovleSensorText(){
    if(mModel.getShtResetting()){
        return "Reset";
    }
    else if(!mModel.getShtPowerEnabled() || !mModel.getShtDriverEnabled()){
        return "Off";
    }
    else if(!mModel.getShtHasSample()){
        return "Busy";
    }
    return "Ok";
}

void MainScreen::onApplyFanStatus(){
    FanState state = mModel.getFanState();
    if(state == FanState::Stalled){
        mFanLabel.setText("Stall");
    }
    else if(state == FanState::Disabled){
        mFanLabel.setText("Off");
    }
    else {
        uint32_t rpm = mModel.getFanMeasuredRpm();
        mFanLabel.setTextFormatted("%u RPM", rpm);
    }
}

void MainScreen::onApplyFanTargetSpeed(){
    // nothing to do
}

void MainScreen::onApplyHeatpadStatus(){
    HeatpadState state = mModel.getHeatpadState();
    if(state == HeatpadState::Disabled){
        mHeatLabel.setText("Off");
    }
    else {
        mHeatLabel.setTextFormatted("%.1f%%", mModel.getHeatpadCurrentDuty()*100.0f);
    }
}

void MainScreen::onApplyHeatpadMeasure(){
    HeatpadState state = mModel.getHeatpadState();
    if(state == HeatpadState::Disabled){
        mPowerLabel.setText("Off");
    }
    else {
        const float voltage = mModel.getHeatpadMeasuredVoltage();
        const float current = mModel.getHeatpadMeasuredCurrent();
        const float duty = mModel.getHeatpadCurrentDuty();
        const float power = voltage * current * duty;
        mPowerLabel.setTextFormatted("%.1f W", power);
    }
}

void MainScreen::onApplyDisplayBrightness(){
    // nothing to do
}

void MainScreen::onApplySensorStatus(){
    // nothing to do
}

void MainScreen::onApplyMeasuredTemperature(){
    if(isSensorOk()){
        mTemperatureLabel.setTextFormatted("%.1f°C", mModel.getMeasuredTemperature());
    }
    else {
        mTemperatureLabel.setText(resovleSensorText());
    }
}

void MainScreen::onApplyMeasuredHumidity(){
    if(isSensorOk()){
        mHumidityLabel.setTextFormatted("%.1f%%", mModel.getMeasuredHumidity());
    }
    else {
        mHumidityLabel.setText(resovleSensorText());
    }
}

void MainScreen::onApplyTargetTemperature(){
    // nothing to do
}

void MainScreen::onApplyEngineState(){
    // nothing to do
}

} // namespace Garbox