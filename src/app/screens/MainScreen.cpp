#include "MainScreen.h"

#include <esp_heap_caps.h>
#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "core/lvgl/LvglContext.h"

namespace Garbox {

MainScreen::MainScreen(): 
    MainScreenAbs(),
    mLvgl(PartsProvider::GetLvglContext()),
    mFanStateLabel(mLvgl.getRoot()),
    mFanMeasuredRpmLabel(mLvgl.getRoot()),
    mHeatpadStateLabel(mLvgl.getRoot()),
    mHeatpadDutyLabel(mLvgl.getRoot()),
    mHeatpadSenseLabel(mLvgl.getRoot()),
    mDisplayStatusLabel(mLvgl.getRoot()),
    mTemperatureStateLabel(mLvgl.getRoot()),
    mTemperatureSampleLabel(mLvgl.getRoot()),
    mHeapSpaceLabel(mLvgl.getRoot()),
    mAppInfoLabel(mLvgl.getRoot()),
    mFermentationStatusLabel(mLvgl.getRoot()){
    // nothing to do
}

void MainScreen::initLabel(LvLabel& label, int16_t x, int16_t y, const char* text) {
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
    initLabel(mDisplayStatusLabel,       startXPx, currentYPx, "Render skipped count:"); currentYPx += deltaYPx;
    initLabel(mFermentationStatusLabel, startXPx, currentYPx, "Eng:"); currentYPx += deltaYPx;
    initLabel(mTemperatureStateLabel,   startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    initLabel(mTemperatureSampleLabel,  startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    initLabel(mFanStateLabel,           startXPx, currentYPx, "Fan state"); currentYPx += deltaYPx;
    initLabel(mFanMeasuredRpmLabel,     startXPx, currentYPx, "Fan"); currentYPx += deltaYPx;
    initLabel(mHeatpadStateLabel,       startXPx, currentYPx, "Heatpad"); currentYPx += deltaYPx;
    initLabel(mHeatpadSenseLabel,       startXPx, currentYPx, "Heatpad");currentYPx += deltaYPx;
    initLabel(mHeatpadDutyLabel,        startXPx, currentYPx, "Duty%:"); currentYPx += deltaYPx;
}

void MainScreen::onStart(){
    mHeapTimer.start(1000_ms);
}

void MainScreen::onBecomeEnabled(){
    mFirstUpdate = true;
}

void MainScreen::onBecomeDisabled(){
    // nothing to do
}

void MainScreen::onUpdateScreen(){

    if(mFirstUpdate){
        mLvgl.setBackgroundColor(0x0);
    }

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

    // update complete
    mFirstUpdate = false;
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
        mModel.getHeatpadCurrentDuty(),
        mModel.getHeatpadNextDuty(),
        mModel.getHeatpadCurrentPeriod(),
        mModel.getHeatpadNextPeriod()
    );
}

void MainScreen::onApplyBoxPosition(){
    mLvgl.setBoxPosition(static_cast<float>(mModel.getHeatpadPwmProgress()) / static_cast<float>(mModel.getHeatpadCurrentPeriod()));
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
        mModel.getDisplayBrightness(), 
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
    mHeapSpaceLabel.setTextFormatted("Heap space: %u.%03u kB", mModel.getHeapSpace());
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