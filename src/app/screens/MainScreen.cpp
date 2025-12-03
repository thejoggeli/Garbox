#include "MainScreen.h"

#include <esp_heap_caps.h>
#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"

namespace Garbox {

MainScreen::MainScreen():
    MainScreenAbs(PartsProvider::GetLvglContext()),
    mProgressBox(mContainer),
    mFanStateLabel(mContainer),
    mFanMeasuredRpmLabel(mContainer),
    mHeatpadStateLabel(mContainer),
    mHeatpadDutyLabel(mContainer),
    mHeatpadSenseLabel(mContainer),
    mDisplayStatusLabel(mContainer),
    mTemperatureStateLabel(mContainer),
    mTemperatureSampleLabel(mContainer),
    mHeapSpaceLabel(mContainer),
    mAppInfoLabel(mContainer),
    mFermentationStatusLabel(mContainer){
    // nothing to do
}

void MainScreen::initLabel(LvLabel& label, int16_t x, int16_t y, const char* text) {
    label.setText(text);
    label.setPosition(x, y);
    label.setTextColor(lv_color_hex(0xFFFFFF));
}

void MainScreen::onInit(){
    const int16_t startXPx = 10;
    const int16_t startYPx = 10;
    const int16_t deltaYPx = 20;
    int16_t currentYPx = startYPx;
    initLabel(mAppInfoLabel,            startXPx, currentYPx, "App:"); currentYPx += deltaYPx;
    initLabel(mHeapSpaceLabel,          startXPx, currentYPx, "Heap space:"); currentYPx += deltaYPx;
    initLabel(mDisplayStatusLabel,      startXPx, currentYPx, "Render skipped count:"); currentYPx += deltaYPx;
    initLabel(mFermentationStatusLabel, startXPx, currentYPx, "Eng:"); currentYPx += deltaYPx;
    initLabel(mTemperatureStateLabel,   startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    initLabel(mTemperatureSampleLabel,  startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    initLabel(mFanStateLabel,           startXPx, currentYPx, "Fan state"); currentYPx += deltaYPx;
    initLabel(mFanMeasuredRpmLabel,     startXPx, currentYPx, "Fan"); currentYPx += deltaYPx;
    initLabel(mHeatpadStateLabel,       startXPx, currentYPx, "Heatpad"); currentYPx += deltaYPx;
    initLabel(mHeatpadSenseLabel,       startXPx, currentYPx, "Heatpad");currentYPx += deltaYPx;
    initLabel(mHeatpadDutyLabel,        startXPx, currentYPx, "Duty%:"); currentYPx += deltaYPx;

    mProgressBox.setSize(48, 8);
    mProgressBox.setBgColor(lv_color_hex(0xFFFFFF));
    mProgressBox.setBgOpacity(LV_OPA_COVER);

    setBackgroundColor(0x0);
}

void MainScreen::onStart(){
    mHeapTimer.start(1000_ms);
}

void MainScreen::onBecomeEnabled(){
    // nothing to do
}

void MainScreen::onBecomeDisabled(){
    // nothing to do
}

void MainScreen::onUpdateScreen(){

    // update heap space
    if(mHeapTimer.isExpired()){
        mModel.setHeapSpace(heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        mHeapTimer.restart();
    }

    // update event count
    mModel.setEventCount(getContext()->eventCount);

    // last dispatched count must be updated manually, otherwise a dispatch is
    // triggered by itself on each tick
    if(mLastDispatchedCount != getDispatchedCount()){
        mLastDispatchedCount = getDispatchedCount();
        if(!isMarkedDirty(Model::Index::DisplayStatus)){
            onApplyDisplayStatus();
        }
    }
}

void MainScreen::onApplyFanState(){
    mFanStateLabel.setTextFormatted(
        "Fan state=%s, speed=%.1f%%", 
        FanStateToString(mModel.getFanState()),
        mModel.getFanTargetSpeed()*100.0f
    );
}

void MainScreen::onApplyFanMeasuredRpm(){
    mFanMeasuredRpmLabel.setTextFormatted("Fan rpm=%.0f", mModel.getFanMeasuredRpm());
}

void MainScreen::onApplyHeatpadState(){
    mHeatpadStateLabel.setTextFormatted(
        "Heatpad state=%s", 
        HeatpadStateToString(mModel.getHeatpadState())
    );
}

void MainScreen::onApplyHeatpadDuty(){
    mHeatpadDutyLabel.setTextFormatted(
        "Duty%: %.0f%% => %.0f%%, ms: %u => %u",
        mModel.getHeatpadCurrentDuty()*100.0f,
        mModel.getHeatpadNextDuty()*100.0f,
        mModel.getHeatpadCurrentPeriod()/1000,
        mModel.getHeatpadNextPeriod()/1000
    );
}

void MainScreen::onApplyBoxPosition(){
    float position = static_cast<float>(mModel.getHeatpadPwmProgress()) / static_cast<float>(mModel.getHeatpadCurrentPeriod());
    static constexpr uint32_t y = 240-8;
    static constexpr float wDisplay = 320.0f; 
    static constexpr float wBox = 48.0f;
    static constexpr float left = -wBox;
    static constexpr float right = wDisplay;
    const float x = std::clamp(position * (right - left) + left, left, right);
    mProgressBox.setPosition(x, y);
}

void MainScreen::onApplyHeatpadSense(){
    mHeatpadSenseLabel.setTextFormatted(
        "Heatpad U=%4.1fV, I=%3.1fA", 
        mModel.getHeatpadMeasuredVoltage(), 
        mModel.getHeatpadMeasuredCurrent()
    );
}

void MainScreen::onApplyDisplayStatus(){
    mDisplayStatusLabel.setTextFormatted(
        "Display: b=%.1f%%, skip=%u, dirty=%u",
        mModel.getDisplayBrightness()*100.0f, 
        mModel.getDisplaySkipped(), 
        getDispatchedCount()
    );
}

void MainScreen::onApplyTemperatureState(){
    mTemperatureStateLabel.setTextFormatted(
        "Sht31 power=%u, driver=%u, reset=%u",
        mModel.getShtPowerEnabled(),
        mModel.getShtDriverEnabled(),
        mModel.getShtResetting()
    );
}

void MainScreen::onApplyTemperatureSample(){
    mTemperatureSampleLabel.setTextFormatted(
        "Sht31 t=%.2f°C, rh=%.2f%%",
        mModel.getSensorTemperatureCelcius(),
        mModel.getSensorHumidityRelative()
    );
}

void MainScreen::onApplyHeapSpace(){
    const uint32_t integer = mModel.getHeapSpace()/1000;
    const uint32_t fraction = mModel.getHeapSpace()%1000;
    mHeapSpaceLabel.setTextFormatted("Heap space: %u.%03u kB", integer, fraction);
}

void MainScreen::onApplyAppInfo(){
    mAppInfoLabel.setTextFormatted(
        "App: %s, events=%u",
        BehaviourIdToString(mModel.getBehaviour()), 
        mModel.getEventCount()
    );
}

void MainScreen::onApplyFermentationStatus(){
    mFermentationStatusLabel.setTextFormatted(
        "Eng: s=%s, m=%.1f°C, t=%0.1f°C",
        HeaterEngineStateToString(mModel.getEngineState()),
        mModel.getEngineMeasuredTemperature(),
        mModel.getEngineTargetTemperature()
    );
}

} // namespace Garbox