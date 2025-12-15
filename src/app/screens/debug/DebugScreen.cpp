#include "DebugScreen.h"

#include <esp_heap_caps.h>
#include <esp_system.h>
#include "core/log/Log.h"
#include "core/util/helpers/StringUtils.h"

namespace Garbox {

DebugScreen::DebugScreen():
    DebugScreenAbs(),
    mProgressBox(mRoot),
    mTimeLabel(mRoot),
    mFanStateLabel(mRoot),
    mFanMeasuredRpmLabel(mRoot),
    mHeatpadStateLabel(mRoot),
    mHeatpadDutyLabel(mRoot),
    mHeatpadSenseLabel(mRoot),
    mDisplayStatusLabel(mRoot),
    mTemperatureStateLabel(mRoot),
    mTemperatureSampleLabel(mRoot),
    mHeapBlocksLabel(mRoot),
    mHeapBytesLabel(mRoot),
    mHeapMinimumLabel(mRoot),
    mAppInfoLabel(mRoot),
    mFermentationStatusLabel(mRoot){
    // nothing to do
}

void DebugScreen::initLabel(LvLabel& label, int16_t x, int16_t y, const char* text) {
    label.setText(text);
    label.setPosition(x, y);
    label.setTextColor(lv_color_hex(0xFFFFFF));
    label.setFont(&lv_font_montserrat_12);
}

void DebugScreen::onInit(){
    const int16_t startXPx = 10;
    const int16_t startYPx = 10;
    const int16_t deltaYPx = 15;
    int16_t currentYPx = startYPx;
    initLabel(mTimeLabel,               startXPx, currentYPx, "Time:"); currentYPx += deltaYPx;
    initLabel(mAppInfoLabel,            startXPx, currentYPx, "App:"); currentYPx += deltaYPx;
    initLabel(mHeapBytesLabel,          startXPx, currentYPx, "Heap:"); currentYPx += deltaYPx;
    initLabel(mHeapMinimumLabel,        startXPx, currentYPx, "Heap:"); currentYPx += deltaYPx;
    initLabel(mHeapBlocksLabel,         startXPx, currentYPx, "Blocks:"); currentYPx += deltaYPx;
    initLabel(mDisplayStatusLabel,      startXPx, currentYPx, "Render skipped count:"); currentYPx += deltaYPx;
    initLabel(mFermentationStatusLabel, startXPx, currentYPx, "Eng:"); currentYPx += deltaYPx;
    initLabel(mTemperatureStateLabel,   startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    initLabel(mTemperatureSampleLabel,  startXPx, currentYPx, "Sht31"); currentYPx += deltaYPx;
    initLabel(mFanStateLabel,           startXPx, currentYPx, "Fan state"); currentYPx += deltaYPx;
    initLabel(mFanMeasuredRpmLabel,     startXPx, currentYPx, "Fan"); currentYPx += deltaYPx;
    initLabel(mHeatpadStateLabel,       startXPx, currentYPx, "Heatpad"); currentYPx += deltaYPx;
    initLabel(mHeatpadSenseLabel,       startXPx, currentYPx, "Heatpad");currentYPx += deltaYPx;
    initLabel(mHeatpadDutyLabel,        startXPx, currentYPx, "Duty%:"); currentYPx += deltaYPx;

    mProgressBox.setRawSize(48, 8);
    mProgressBox.setBgColor(lv_color_hex(0xFFFFFF));
    mProgressBox.setBgOpa(LV_OPA_COVER);

    setBackgroundColor(0x0);
}

void DebugScreen::onStart(){
    mHeapTimer.start(1000_ms);
}

void DebugScreen::onBecomeEnabled(){
    // nothing to do
}

void DebugScreen::onBecomeDisabled(){
    // nothing to do
}

void DebugScreen::onUpdateScreen(){

    // update time
    model().setTimeSeconds(Time::GetTickSeconds());

    // update heap space
    if(mHeapTimer.isExpired()){
        multi_heap_info_t info;
        heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);
        model().setHeapAllocatedBlocks(info.allocated_blocks);
        model().setHeapFreeBlocks(info.free_blocks);
        model().setHeapLargestFreeBlock(info.largest_free_block);
        if(info.minimum_free_bytes != model().getHeapMinimumFreeBytes()){
            model().setHeapMinimumTime(Time::GetTickSeconds());
            model().setHeapMinimumFreeBytes(info.minimum_free_bytes);
        }
        model().setHeapTotalFreeBytes(info.total_free_bytes);
        model().setHeapAllocatedBytes(info.total_allocated_bytes);
        mHeapTimer.restart();
    }

    // update event count
    model().setEventCount(getContext()->eventCount);

    // last dispatched count must be updated manually, otherwise a dispatch is
    // triggered by itself on each tick
    if(mLastDispatchedCount != getDispatchedCount()){
        mLastDispatchedCount = getDispatchedCount();
        if(!isMarkedDirty(Model::Index::DisplayStatus)){
            onApplyDisplayStatus();
        }
    }
}

void DebugScreen::onApplyFanState(){
    mFanStateLabel.setTextFormatted(
        "Fan state=%s, speed=%.1f%%", 
        FanStateToString(model().getFanState()),
        model().getFanTargetSpeed()*100.0f
    );
}

void DebugScreen::onApplyFanMeasuredRpm(){
    mFanMeasuredRpmLabel.setTextFormatted("Fan rpm=%.0f", model().getFanMeasuredRpm());
}

void DebugScreen::onApplyHeatpadState(){
    mHeatpadStateLabel.setTextFormatted(
        "Heatpad state=%s", 
        HeatpadStateToString(model().getHeatpadState())
    );
}

void DebugScreen::onApplyHeatpadDuty(){
    mHeatpadDutyLabel.setTextFormatted(
        "Duty%: %.0f%% => %.0f%%, ms: %u => %u",
        model().getHeatpadCurrentDuty()*100.0f,
        model().getHeatpadNextDuty()*100.0f,
        model().getHeatpadCurrentPeriod()/1000,
        model().getHeatpadNextPeriod()/1000
    );
}

void DebugScreen::onApplyBoxPosition(){
    float position = static_cast<float>(model().getHeatpadPwmProgress()) / static_cast<float>(model().getHeatpadCurrentPeriod());
    static constexpr uint32_t y = 240-4;
    static constexpr float wDisplay = 320.0f; 
    static constexpr float wBox = 48.0f;
    static constexpr float left = -wBox;
    static constexpr float right = wDisplay;
    const float x = std::clamp(position * (right - left) + left, left, right);
    mProgressBox.setPosition(x, y);
}

void DebugScreen::onApplyHeatpadSense(){
    mHeatpadSenseLabel.setTextFormatted(
        "Heatpad U=%4.1fV, I=%3.1fA", 
        model().getHeatpadMeasuredVoltage(), 
        model().getHeatpadMeasuredCurrent()
    );
}

void DebugScreen::onApplyDisplayStatus(){
    mDisplayStatusLabel.setTextFormatted(
        "Display: b=%.1f%%, skip=%u, dirty=%u",
        model().getDisplayBrightness()*100.0f, 
        model().getDisplaySkipped(), 
        getDispatchedCount()
    );
}

void DebugScreen::onApplyTemperatureState(){
    mTemperatureStateLabel.setTextFormatted(
        "Sht31 power=%u, driver=%u, reset=%u",
        model().getShtPowerEnabled(),
        model().getShtDriverEnabled(),
        model().getShtResetting()
    );
}

void DebugScreen::onApplyTemperatureSample(){
    mTemperatureSampleLabel.setTextFormatted(
        "Sht31 t=%.2f°C, rh=%.2f%%",
        model().getSensorTemperatureCelcius(),
        model().getSensorHumidityRelative()
    );
}

void DebugScreen::onApplyTime(){
    static char buffer[32];
    StringUtils::FormatDurationDHMS(model().getTimeSeconds(), buffer, 32);
    mTimeLabel.setTextFormatted("Time: %s", buffer);
}

void DebugScreen::onApplyHeapBlocks(){
    const uint32_t integer = model().getHeapLargestFreeBlock()/1000;
    const uint32_t fraction = model().getHeapLargestFreeBlock()%1000;
    mHeapBlocksLabel.setTextFormatted("Blocks: free=%u, alloc=%u, largest=%u.%03u kB", 
        model().getHeapFreeBlocks(),
        model().getHeapAllocatedBlocks(),
        integer, fraction
    );
}

void DebugScreen::onApplyHeapBytes(){
    const uint32_t int1 = model().getHeapTotalFreeBytes()/1000;
    const uint32_t frac1 = model().getHeapTotalFreeBytes()%1000;
    const uint32_t int2 = model().getHeapAllocatedBytes()/1000;
    const uint32_t frac2 = model().getHeapAllocatedBytes()%1000;
    mHeapBytesLabel.setTextFormatted("Heap: free=%u.%03u kB, alloc=%u.%03u kB", 
        int1, frac1,
        int2, frac2
    );
}

void DebugScreen::onApplyHeapMinimum(){
    const uint32_t int1 = model().getHeapMinimumFreeBytes()/1000;
    const uint32_t frac1 = model().getHeapMinimumFreeBytes()%1000;
    static char buffer[32];
    StringUtils::FormatDurationDHMS(model().getHeapMinimumTime(), buffer, 32);
    mHeapMinimumLabel.setTextFormatted("Heap: min=%u.%03u kB, time=%s", 
        int1, frac1, buffer
    );
}

void DebugScreen::onApplyAppInfo(){
    mAppInfoLabel.setTextFormatted(
        "App: %s, events=%u",
        BehaviourIdToString(model().getBehaviour()), 
        model().getEventCount()
    );
}

void DebugScreen::onApplyFermentationStatus(){
    mFermentationStatusLabel.setTextFormatted(
        "Eng: s=%s, m=%.1f°C, t=%0.1f°C",
        HeaterEngineStateToString(model().getEngineState()),
        model().getEngineMeasuredTemperature(),
        model().getEngineTargetTemperature()
    );
}

} // namespace Garbox