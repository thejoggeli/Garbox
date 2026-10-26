#include "DebugScreen.h"

#include <algorithm>
#include <cmath>
#include "app/components/screens/ScreenCommandBar.h"
#include "core/log/Log.h"
#include "core/util/helpers/StringUtils.h"

namespace Garbox {

DebugScreen::DebugScreen():
    DebugScreenAbs(),
    mHeaderLabel(mRoot),
    mViewHint(mRoot),
    mScrollViewport(mRoot),
    mFooter(mRoot),
    mFooterPressHint(mFooter),
    mFooterHoldHint(mFooter),
    mSystemCard(mScrollViewport),
    mMemoryCard(mScrollViewport),
    mClimateCard(mScrollViewport),
    mOutputsCard(mScrollViewport),
    mProgressTrack(mOutputsCard.body),
    mProgressBox(static_cast<LvObject&>(mProgressTrack)),
    mMicrosLabel(mSystemCard.body),
    mTimeLabel(mSystemCard.body),
    mFanStateLabel(mOutputsCard.body),
    mFanMeasuredRpmLabel(mOutputsCard.body),
    mHeatpadStateLabel(mOutputsCard.body),
    mHeatpadDutyLabel(mOutputsCard.body),
    mHeatpadSenseLabel(mOutputsCard.body),
    mDisplayStatusLabel(mSystemCard.body),
    mTemperatureStateLabel(mClimateCard.body),
    mTemperatureSampleLabel(mClimateCard.body),
    mHeapBlocksLabel(mMemoryCard.body),
    mHeapBytesLabel(mMemoryCard.body),
    mHeapMinimumLabel(mMemoryCard.body),
    mAppInfoLabel(mSystemCard.body),
    mFermentationStatusLabel(mClimateCard.body){
    // nothing to do
}

void DebugScreen::initCard(
    DiagnosticCard& card,
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    const char* title,
    uint32_t accentColor){

    card.body.setPosition(x, y);
    card.body.setRawSize(width, height);
    card.body.setBgColor(lv_color_hex(0x0D141A));
    card.body.setBgOpa(LV_OPA_COVER);
    card.body.setBorder(1, lv_color_hex(0x26343D));
    card.body.setRadius(7);
    card.body.setScrollable(false);
    lv_obj_set_scrollbar_mode(card.body.raw(), LV_SCROLLBAR_MODE_OFF);

    card.title.setText(title);
    card.title.setPosition(10, 7);
    card.title.setFont(&lv_font_unscii_8);
    card.title.setTextColor(lv_color_hex(accentColor));
}

void DebugScreen::initLabel(LvLabel& label, int16_t x, int16_t y, const char* text) {
    label.setText(text);
    label.setPosition(x, y);
    label.setTextColor(lv_color_hex(0xFFFFFF));
    label.setFont(&lv_font_unscii_8);
    label.setTextLineSpace(3);
}

void DebugScreen::onInit(){
    mHeaderLabel.setText("DIAGNOSTICS");
    mHeaderLabel.setPosition(10, 7);
    mHeaderLabel.setFont(&lv_font_montserrat_12);
    mHeaderLabel.setTextColor(lv_color_hex(0x8AB4F4));

    mViewHint.setText("ROTATE: SCROLL");
    mViewHint.setPosition(170, 9);
    mViewHint.setWidth(140);
    mViewHint.setTextAlign(LV_TEXT_ALIGN_RIGHT);
    mViewHint.setFont(&lv_font_unscii_8);
    mViewHint.setTextColor(lv_color_hex(0x8AB4F4));

    ConfigureScreenCommandBar(
        mFooter,
        mFooterPressHint,
        mFooterHoldHint,
        "PRESS: EVENT LOG",
        "HOLD: MAIN"
    );

    mScrollViewport.setPosition(0, 27);
    mScrollViewport.setRawSize(AppConfig::DisplayWidth, 193);
    mScrollViewport.setBgOpa(LV_OPA_TRANSP);
    mScrollViewport.setBorderWidth(0);
    mScrollViewport.setPadAll(0);
    mScrollViewport.setScrollable(true);
    mScrollViewport.setScrollDirection(LV_DIR_VER);
    mScrollViewport.setScrollMomentum(false);
    mScrollViewport.setScrollElastic(false);
    lv_obj_set_scrollbar_mode(mScrollViewport.raw(), LV_SCROLLBAR_MODE_ON);
    lv_obj_set_style_width(mScrollViewport.raw(), 3, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(mScrollViewport.raw(), lv_color_hex(0x8AB4F4), LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_opa(mScrollViewport.raw(), LV_OPA_70, LV_PART_SCROLLBAR);
    lv_obj_set_style_radius(mScrollViewport.raw(), LV_RADIUS_CIRCLE, LV_PART_SCROLLBAR);

    // Full-width, vertically stacked cards trade density for a stable label
    // column and leave room for future diagnostic fields.
    initCard(mSystemCard,   6,   4, 304, 143, "SYSTEM",  0x8AB4F4);
    initCard(mClimateCard,  6, 153, 304, 118, "CLIMATE", 0x75C8C8);
    initCard(mOutputsCard,  6, 277, 304, 150, "OUTPUTS", 0xD1B254);
    initCard(mMemoryCard,   6, 433, 304, 120, "MEMORY",  0xBDA7FF);

    initLabel(mTimeLabel,               10,  29, "UPTIME");
    initLabel(mMicrosLabel,             10,  44, "CLOCK");
    initLabel(mAppInfoLabel,            10,  59, "BEHAVIOR");
    initLabel(mDisplayStatusLabel,      10, 105, "BRIGHTNESS");
    initLabel(mFermentationStatusLabel, 10,  29, "MODE");
    initLabel(mTemperatureStateLabel,   10,  54, "POWER");
    initLabel(mTemperatureSampleLabel,  10,  89, "TEMPERATURE");
    initLabel(mFanStateLabel,           10,  29, "FAN STATE");
    initLabel(mFanMeasuredRpmLabel,     10,  54, "FAN SPEED");
    initLabel(mHeatpadStateLabel,       10,  69, "HEATER");
    initLabel(mHeatpadSenseLabel,       10,  84, "VOLTAGE");
    initLabel(mHeatpadDutyLabel,        10, 109, "DUTY NOW");
    initLabel(mHeapBytesLabel,          10,  29, "FREE");
    initLabel(mHeapMinimumLabel,        10,  54, "MIN FREE");
    initLabel(mHeapBlocksLabel,         10,  79, "FREE BLOCKS");

    mProgressTrack.setPosition(10, 137);
    mProgressTrack.setRawSize(284, 4);
    mProgressTrack.setBgColor(lv_color_hex(0x26343D));
    mProgressTrack.setBgOpa(LV_OPA_COVER);

    mProgressBox.setPosition(0, 0);
    mProgressBox.setRawSize(1, 4);
    mProgressBox.setBgColor(lv_color_hex(0xD1B254));
    mProgressBox.setBgOpa(LV_OPA_COVER);

    setBackgroundColor(0x070B0E);
    mRoot.setScrollable(false);
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
    if(event->newState == ButtonState::PressedLong){
        host()->requestChangeScreen(ScreenId::Main);
    }
    else if(event->newState == ButtonState::Released && event->oldState == ButtonState::Pressed){
        host()->requestChangeScreen(ScreenId::EventLog);
    }
}

void DebugScreen::onEncoderStepEvent(const EncoderStepEvent& event){
    if(event->steps != 0){
        static constexpr int32_t ScrollPixelsPerStep = 28;
        const int32_t targetY = lv_obj_get_scroll_y(mScrollViewport.raw())
            + event->steps * ScrollPixelsPerStep;
        lv_obj_scroll_to_y(
            mScrollViewport.raw(),
            targetY,
            LV_ANIM_OFF
        );
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
        "FAN STATE     %s\nFAN TARGET    %.0f %%",
        FanStateToString(states().fanStatus.getState()),
        states().fanStatus.getTargetSpeed()*100.0f
    );
}

void DebugScreen::onRenderFanMeasuredRpm(){
    mFanMeasuredRpmLabel.setTextFormatted("FAN SPEED     %.0f RPM", states().fanSample.getMeasuredRpm());
}

void DebugScreen::onRenderHeatpadState(){
    mHeatpadStateLabel.setTextFormatted(
        "HEATER        %s",
        HeatpadStateToString(states().heatpadStatus.getState())
    );
}

void DebugScreen::onRenderHeatpadDuty(){
    const float appliedDuty = states().heatpadStatus.getCurrentDutyCycle() * 100.0f;
    const float queuedDuty = states().heatpadStatus.getNextDutyCycle() * 100.0f;
    const uint32_t appliedPeriod = states().heatpadStatus.getCurrentPeriodMicros() / 1000;
    const uint32_t queuedPeriod = states().heatpadStatus.getNextPeriodMicros() / 1000;

    const bool dutyIsQueued = std::fabs(appliedDuty - queuedDuty) >= 0.05f;
    const bool periodIsQueued = appliedPeriod != queuedPeriod;
    if(dutyIsQueued || periodIsQueued){
        mHeatpadDutyLabel.setTextFormatted(
            "DUTY         %.1f -> %.1f %%\nPERIOD       %u -> %u ms",
            appliedDuty,
            queuedDuty,
            appliedPeriod,
            queuedPeriod
        );
    }
    else {
        mHeatpadDutyLabel.setTextFormatted(
            "DUTY         %.1f %%\nPERIOD       %u ms",
            appliedDuty,
            appliedPeriod
        );
    }
}

void DebugScreen::onRenderHeatpadSense(){
    mHeatpadSenseLabel.setTextFormatted(
        "VOLTAGE       %.1f V\nCURRENT       %.1f A",
        states().heatpadSample.getMeasuredVoltage(), 
        states().heatpadSample.getMeasuredCurrent()
    );
}

void DebugScreen::onRenderDisplayStatus(){
    mDisplayStatusLabel.setTextFormatted(
        "BRIGHTNESS    %.0f %%\nSKIPPED       %u\nDIRTY         %u",
        states().displayStatus.getBrightness()*100.0f, 
        states().displayDiagnostics.getSkippedFrames(), 
        getDispatchedCount()
    );
}

void DebugScreen::onRenderHeatpadProgress(){
    float progress = states().heatpadProgress.getPwmProgressMicros();
    float period = states().heatpadStatus.getCurrentPeriodMicros();
    float position = period > 0.0f ? progress / period : 0.0f;
    static constexpr int32_t width = 284;
    const float clamped = std::clamp(position, 0.0f, 1.0f);
    const int32_t fillWidth = static_cast<int32_t>(clamped * width + 0.5f);
    mProgressBox.setHidden(fillWidth == 0);
    mProgressBox.setRawSize(std::max(fillWidth, 1), 3);
}

void DebugScreen::onRenderTemperatureState(){
    mTemperatureStateLabel.setTextFormatted(
        "POWER         %s\nDRIVER        %s\nRESET         %s",
        states().temperatureStatus.getPowerEnabled() ? "ON" : "OFF",
        states().temperatureStatus.getDriverEnabled() ? "ON" : "OFF",
        states().temperatureStatus.getResetting() ? "YES" : "NO"
    );
}

void DebugScreen::onRenderTemperatureSample(){
    mTemperatureSampleLabel.setTextFormatted(
        "TEMPERATURE   %.2f C\nHUMIDITY      %.2f %%",
        states().temperatureSample.getTemperatureCelcius(),
        states().temperatureSample.getHumidityRelative()
    );
}

void DebugScreen::onRenderTime(){
    static char buffer[32];
    StringUtils::FormatDurationDHMS(mLastTimeSeconds, buffer, 32);
    mTimeLabel.setTextFormatted("UPTIME        %s", buffer);
}

void DebugScreen::onRenderMicros(){
    uint32_t t0 = Time::GetTickMicros() % 1000;
    uint32_t t1 = (Time::GetTickMicros() / 1000) % 1000;
    uint32_t t2 = (Time::GetTickMicros() / 1000000) % 1000;
    uint32_t t3 = (Time::GetTickMicros() / 1000000000);
    mMicrosLabel.setTextFormatted("CLOCK         %03u'%03u'%03u'%03u", t3, t2, t1, t0);
}

void DebugScreen::onRenderHeapBlocks(){
    const uint32_t integer = mHeapInfo.largest_free_block/1000;
    const uint32_t fraction = mHeapInfo.largest_free_block%1000;
    mHeapBlocksLabel.setTextFormatted("FREE BLOCKS   %u\nUSED BLOCKS   %u\nLARGEST       %u.%03u kB",
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
    mHeapBytesLabel.setTextFormatted("FREE          %u.%03u kB\nUSED          %u.%03u kB",
        int1, frac1,
        int2, frac2
    );
}

void DebugScreen::onRenderHeapMinimum(){
    const uint32_t int1 = mHeapInfo.minimum_free_bytes/1000;
    const uint32_t frac1 = mHeapInfo.minimum_free_bytes%1000;
    static char buffer[32];
    StringUtils::FormatDurationDHMS(mHeapMinimumTime, buffer, 32);
    mHeapMinimumLabel.setTextFormatted("MIN FREE      %u.%03u kB\nMIN AT        %s",
        int1, frac1, buffer
    );
}

void DebugScreen::onRenderAppInfo(){
    mAppInfoLabel.setTextFormatted(
        "BEHAVIOR      %s\nSCREEN        %s\nEVENTS        %u\nSTATE CHG     %u",
        BehaviourIdToString(states().activeBehaviour.getBehaviour()), 
        ScreenIdToString(states().activeScreen.getScreen()),
        mLastEventCount, mLastStateChangesCount
    );
}

void DebugScreen::onRenderFermentationStatus(){
    mFermentationStatusLabel.setTextFormatted(
        "MODE          %s\nTARGET        %.1f C",
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
    markDirty(RenderFn::AppInfo);
}

void DebugScreen::onFermentationStatusStateChanged(const FermentationStatusState& state){
    markDirty(RenderFn::FermentationStatus);
}

} // namespace Garbox
