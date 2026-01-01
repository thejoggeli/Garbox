#include "DebugScreen.h"

#include "core/log/Log.h"
#include "core/util/helpers/StringUtils.h"

namespace Garbox {

DebugScreen::DebugScreen():
    DebugScreenAbs(),
    mProgressBox(mRoot),
    mMicrosLabel(mRoot),
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

    initLabel(mMicrosLabel,             startXPx, currentYPx, "us");
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

    mMicrosLabel.setTextAlign(LV_TEXT_ALIGN_RIGHT);
    mMicrosLabel.setPositionX(0);
    mMicrosLabel.setWidth(mScreenWidth);
    mMicrosLabel.setFont(&lv_font_unscii_8);
    mMicrosLabel.setPadRight(startXPx);

    mTimeLabel.setFont(&lv_font_unscii_8);

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

void DebugScreen::onButtonEvent(const ButtonEvent& event){
    if(event->newState == ButtonState::Released && event->oldState == ButtonState::Pressed){
        host()->requestChangeScreen(ScreenId::EventLog);
    }
}

void DebugScreen::onRender(){

    // update time 
    uint32_t timeSeconds = Time::GetTickSeconds();
    if(mLastTimeSeconds != timeSeconds){
        mLastTimeSeconds = timeSeconds;
        markDirty(RenderFn::Time);
    }
    
    // update micros
    markDirty(RenderFn::Micros);

    // update heap space
    if(mHeapTimer.isExpired()){
        multi_heap_info_t info;
        heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);
        if(info.minimum_free_bytes != mHeapInfo.minimum_free_bytes){
            mHeapMinimumTime = Time::GetTickSeconds();
        }
        mHeapInfo = info;
        markDirty(RenderFn::HeapBlocks);
        markDirty(RenderFn::HeapBytes);
        markDirty(RenderFn::HeapMinimum);
        mHeapTimer.restart();
    }

    // update event count
    uint32_t eventCount = getContext()->eventCount;
    if(mLastEventCount != eventCount){
        mLastEventCount = eventCount;
        markDirty(RenderFn::AppInfo);
    }

    // update state changes count
    uint32_t stateChangesCount = getContext()->stateChangesCount;
    if(mLastStateChangesCount != stateChangesCount){
        mLastStateChangesCount = stateChangesCount;
        markDirty(RenderFn::AppInfo);
    }
}

void DebugScreen::onRenderFanState(){
    mFanStateLabel.setTextFormatted(
        "Fan state=%s, speed=%.1f%%", 
        FanStateToString(states().fanStatus.getState()),
        states().fanStatus.getTargetSpeed()*100.0f
    );
}

void DebugScreen::onRenderFanMeasuredRpm(){
    mFanMeasuredRpmLabel.setTextFormatted("Fan rpm=%.0f", states().fanSample.getMeasuredRpm());
}

void DebugScreen::onRenderHeatpadState(){
    mHeatpadStateLabel.setTextFormatted(
        "Heatpad state=%s", 
        HeatpadStateToString(states().heatpadStatus.getState())
    );
}

void DebugScreen::onRenderHeatpadDuty(){
    mHeatpadDutyLabel.setTextFormatted(
        "Duty: %.0f%% => %.0f%%, ms: %u => %u",
        states().heatpadStatus.getCurrentDutyCycle()*100.0f,
        states().heatpadStatus.getNextDutyCycle()*100.0f,
        states().heatpadStatus.getCurrentPeriodMicros()/1000,
        states().heatpadStatus.getNextPeriodMicros()/1000
    );
}

void DebugScreen::onRenderHeatpadSense(){
    mHeatpadSenseLabel.setTextFormatted(
        "Heatpad U=%4.1fV, I=%3.1fA", 
        states().heatpadSample.getMeasuredVoltage(), 
        states().heatpadSample.getMeasuredCurrent()
    );
}

void DebugScreen::onRenderDisplayStatus(){
    mDisplayStatusLabel.setTextFormatted(
        "Display: b=%.1f%%, skip=%u, dirty=%u",
        states().displayStatus.getBrightness()*100.0f, 
        states().displayDiagnostics.getSkippedFrames(), 
        getDispatchedCount()
    );
}

void DebugScreen::onRenderHeatpadProgress(){
    float progress = states().heatpadProgress.getPwmProgressMicros();
    float period = states().heatpadStatus.getCurrentPeriodMicros();
    float position = progress / period;
    static constexpr uint32_t y = 240-4;
    static constexpr float wDisplay = 320.0f; 
    static constexpr float wBox = 48.0f;
    static constexpr float left = -wBox;
    static constexpr float right = wDisplay;
    const float x = std::clamp(position * (right - left) + left, left, right);
    mProgressBox.setPosition(x, y);
}

void DebugScreen::onRenderTemperatureState(){
    mTemperatureStateLabel.setTextFormatted(
        "Sht31 power=%u, driver=%u, reset=%u",
        states().temperatureStatus.getPowerEnabled(),
        states().temperatureStatus.getDriverEnabled(),
        states().temperatureStatus.getResetting()
    );
}

void DebugScreen::onRenderTemperatureSample(){
    mTemperatureSampleLabel.setTextFormatted(
        "Sht31 t=%.2f°C, rh=%.2f%%",
        states().temperatureSample.getTemperatureCelcius(),
        states().temperatureSample.getHumidityRelative()
    );
}

void DebugScreen::onRenderTime(){
    static char buffer[32];
    StringUtils::FormatDurationDHMS(mLastTimeSeconds, buffer, 32);
    mTimeLabel.setTextFormatted("%s", buffer);
}

void DebugScreen::onRenderMicros(){
    uint32_t t0 = Time::GetTickMicros() % 1000;
    uint32_t t1 = (Time::GetTickMicros() / 1000) % 1000;
    uint32_t t2 = (Time::GetTickMicros() / 1000000) % 1000;
    uint32_t t3 = (Time::GetTickMicros() / 1000000000);
    mMicrosLabel.setTextFormatted("%03u'%03u'%03u'%03u us", t3, t2, t1, t0);
}

void DebugScreen::onRenderHeapBlocks(){
    const uint32_t integer = mHeapInfo.largest_free_block/1000;
    const uint32_t fraction = mHeapInfo.largest_free_block%1000;
    mHeapBlocksLabel.setTextFormatted("Blocks: free=%u, alloc=%u, largest=%u.%03u kB", 
        mHeapInfo.free_blocks,
        mHeapInfo.allocated_blocks,
        integer, fraction
    );
}

void DebugScreen::onRenderHeapBytes(){
    const uint32_t int1 = mHeapInfo.total_free_bytes/1000;
    const uint32_t frac1 = mHeapInfo.total_free_bytes%1000;
    const uint32_t int2 = mHeapInfo.total_allocated_bytes/1000;
    const uint32_t frac2 = mHeapInfo.total_allocated_bytes%1000;
    mHeapBytesLabel.setTextFormatted("Heap: free=%u.%03u kB, alloc=%u.%03u kB", 
        int1, frac1,
        int2, frac2
    );
}

void DebugScreen::onRenderHeapMinimum(){
    const uint32_t int1 = mHeapInfo.minimum_free_bytes/1000;
    const uint32_t frac1 = mHeapInfo.minimum_free_bytes%1000;
    static char buffer[32];
    StringUtils::FormatDurationDHMS(mHeapMinimumTime, buffer, 32);
    mHeapMinimumLabel.setTextFormatted("Heap: min=%u.%03u kB, time=%s", 
        int1, frac1, buffer
    );
}

void DebugScreen::onRenderAppInfo(){
    mAppInfoLabel.setTextFormatted(
        "Beha=%s, events=%u, states=%u",
        BehaviourIdToString(states().activeBehaviour.getBehaviour()), 
        mLastEventCount, mLastStateChangesCount
    );
}

void DebugScreen::onRenderFermentationStatus(){
    mFermentationStatusLabel.setTextFormatted(
        "Eng: s=%s, t=%0.1f°C",
        FermentationStateToString(states().fermentationStatus.getState()),
        states().fermentationStatus.getTargetTemperature()
    );
}

void DebugScreen::onFanStatusStateChanged(const FanStatusState& state){
    markDirty(RenderFn::FanState);
}

void DebugScreen::onFanSampleStateChanged(const FanSampleState& state){
    markDirty(RenderFn::FanMeasuredRpm);
}

void DebugScreen::onHeatpadStatusStateChanged(const HeatpadStatusState& state){
    markDirty(RenderFn::HeatpadState);
    markDirty(RenderFn::HeatpadProgress);
    markDirty(RenderFn::HeatpadDuty);
}

void DebugScreen::onHeatpadSampleStateChanged(const HeatpadSampleState& state){
    markDirty(RenderFn::HeatpadSense);
    markDirty(RenderFn::HeatpadDuty);
}

void DebugScreen::onHeatpadProgressStateChanged(const HeatpadProgressState& state){
    markDirty(RenderFn::HeatpadProgress);
}

void DebugScreen::onDisplayStatusStateChanged(const DisplayStatusState& state){
    markDirty(RenderFn::DisplayStatus);
}

void DebugScreen::onDisplayDiagnosticsStateChanged(const DisplayDiagnosticsState& state){
    markDirty(RenderFn::DisplayStatus);
}

void DebugScreen::onTemperatureStatusStateChanged(const TemperatureStatusState& state){
    markDirty(RenderFn::TemperatureState);
}

void DebugScreen::onTemperatureSampleStateChanged(const TemperatureSampleState& state){
    markDirty(RenderFn::TemperatureSample);
}

void DebugScreen::onActiveBehaviourStateChanged(const ActiveBehaviourState& state){
    markDirty(RenderFn::AppInfo);
}

void DebugScreen::onActiveScreenStateChanged(const ActiveScreenState& state){
    // add label for active screen
}

void DebugScreen::onFermentationStatusStateChanged(const FermentationStatusState& state){
    markDirty(RenderFn::FermentationStatus);
}

} // namespace Garbox