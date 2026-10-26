#include "DebugScreen.h"

#include <algorithm>
#include "app/components/screens/ScreenCommandBar.h"
#include "core/log/Log.h"
#include "core/util/helpers/StringUtils.h"

namespace Garbox {

DebugScreen::DebugScreen():
    DebugScreenAbs(),
    mHeaderLabel(mRoot),
    mViewHint(mRoot),
    mFooter(mRoot),
    mFooterPressHint(mFooter),
    mFooterHoldHint(mFooter),
    mSystemCard(mRoot),
    mMemoryCard(mRoot),
    mClimateCard(mRoot),
    mOutputsCard(mRoot),
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
    card.title.setPosition(7, 4);
    card.title.setFont(&lv_font_unscii_8);
    card.title.setTextColor(lv_color_hex(accentColor));
}

void DebugScreen::initLabel(LvLabel& label, int16_t x, int16_t y, const char* text) {
    label.setText(text);
    label.setPosition(x, y);
    label.setTextColor(lv_color_hex(0xFFFFFF));
    label.setFont(&lv_font_montserrat_10);
}

void DebugScreen::onInit(){
    mHeaderLabel.setText("DIAGNOSTICS");
    mHeaderLabel.setPosition(8, 6);
    mHeaderLabel.setFont(&lv_font_montserrat_12);
    mHeaderLabel.setTextColor(lv_color_hex(0x8AB4F4));

    mViewHint.setText("ROTATE VIEWS 1/3");
    mViewHint.setPosition(176, 9);
    mViewHint.setWidth(136);
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

    initCard(mSystemCard,  6,   29, 151, 104, "SYSTEM",  0x8AB4F4);
    initCard(mMemoryCard,  6,  137, 151,  77, "MEMORY",  0xBDA7FF);
    initCard(mClimateCard, 163, 29, 151,  77, "CLIMATE", 0x75C8C8);
    initCard(mOutputsCard, 163, 110, 151, 104, "OUTPUTS", 0xD1B254);

    initLabel(mTimeLabel,                7, 20, "UP");
    initLabel(mMicrosLabel,              7, 35, "CLOCK");
    initLabel(mAppInfoLabel,             7, 50, "APP");
    initLabel(mDisplayStatusLabel,       7, 87, "LCD");
    initLabel(mHeapBytesLabel,           7, 18, "FREE");
    initLabel(mHeapMinimumLabel,         7, 36, "MIN");
    initLabel(mHeapBlocksLabel,          7, 59, "BLOCKS");
    initLabel(mFermentationStatusLabel,  7, 20, "ENGINE");
    initLabel(mTemperatureStateLabel,    7, 38, "SENSOR");
    initLabel(mTemperatureSampleLabel,   7, 56, "TEMP");
    initLabel(mFanStateLabel,            7, 18, "FAN");
    initLabel(mFanMeasuredRpmLabel,      7, 33, "RPM");
    initLabel(mHeatpadStateLabel,        7, 48, "HEATER");
    initLabel(mHeatpadSenseLabel,        7, 63, "SENSE");
    initLabel(mHeatpadDutyLabel,         7, 78, "PWM");

    mAppInfoLabel.setFont(&lv_font_unscii_8);
    mHeapBytesLabel.setFont(&lv_font_unscii_8);
    mHeapMinimumLabel.setFont(&lv_font_unscii_8);
    mHeatpadDutyLabel.setFont(&lv_font_unscii_8);

    mProgressTrack.setPosition(7, 98);
    mProgressTrack.setRawSize(137, 3);
    mProgressTrack.setBgColor(lv_color_hex(0x26343D));
    mProgressTrack.setBgOpa(LV_OPA_COVER);

    mProgressBox.setPosition(0, 0);
    mProgressBox.setRawSize(1, 3);
    mProgressBox.setBgColor(lv_color_hex(0xD1B254));
    mProgressBox.setBgOpa(LV_OPA_COVER);

    setBackgroundColor(0x070B0E);
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
    if(event->steps > 0){
        host()->requestChangeScreen(ScreenId::EventLog);
    }
    else if(event->steps < 0){
        host()->requestChangeScreen(ScreenId::StateLog);
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
        "FAN %s  SET %.0f%%",
        FanStateToString(states().fanStatus.getState()),
        states().fanStatus.getTargetSpeed()*100.0f
    );
}

void DebugScreen::onRenderFanMeasuredRpm(){
    mFanMeasuredRpmLabel.setTextFormatted("RPM %.0f", states().fanSample.getMeasuredRpm());
}

void DebugScreen::onRenderHeatpadState(){
    mHeatpadStateLabel.setTextFormatted(
        "HEATER %s",
        HeatpadStateToString(states().heatpadStatus.getState())
    );
}

void DebugScreen::onRenderHeatpadDuty(){
    mHeatpadDutyLabel.setTextFormatted(
        "DUTY %.0f>%.0f%%\nPER %u>%ums",
        states().heatpadStatus.getCurrentDutyCycle()*100.0f,
        states().heatpadStatus.getNextDutyCycle()*100.0f,
        states().heatpadStatus.getCurrentPeriodMicros()/1000,
        states().heatpadStatus.getNextPeriodMicros()/1000
    );
}

void DebugScreen::onRenderHeatpadSense(){
    mHeatpadSenseLabel.setTextFormatted(
        "SENSE %.1fV  %.1fA",
        states().heatpadSample.getMeasuredVoltage(), 
        states().heatpadSample.getMeasuredCurrent()
    );
}

void DebugScreen::onRenderDisplayStatus(){
    mDisplayStatusLabel.setTextFormatted(
        "LCD %.0f%% SKIP %u DIRTY %u",
        states().displayStatus.getBrightness()*100.0f, 
        states().displayDiagnostics.getSkippedFrames(), 
        getDispatchedCount()
    );
}

void DebugScreen::onRenderHeatpadProgress(){
    float progress = states().heatpadProgress.getPwmProgressMicros();
    float period = states().heatpadStatus.getCurrentPeriodMicros();
    float position = period > 0.0f ? progress / period : 0.0f;
    static constexpr int32_t width = 137;
    const float clamped = std::clamp(position, 0.0f, 1.0f);
    const int32_t fillWidth = static_cast<int32_t>(clamped * width + 0.5f);
    mProgressBox.setHidden(fillWidth == 0);
    mProgressBox.setRawSize(std::max(fillWidth, 1), 3);
}

void DebugScreen::onRenderTemperatureState(){
    mTemperatureStateLabel.setTextFormatted(
        "SENSOR P%u D%u RESET%u",
        states().temperatureStatus.getPowerEnabled(),
        states().temperatureStatus.getDriverEnabled(),
        states().temperatureStatus.getResetting()
    );
}

void DebugScreen::onRenderTemperatureSample(){
    mTemperatureSampleLabel.setTextFormatted(
        "%.2f C  RH %.2f%%",
        states().temperatureSample.getTemperatureCelcius(),
        states().temperatureSample.getHumidityRelative()
    );
}

void DebugScreen::onRenderTime(){
    static char buffer[32];
    StringUtils::FormatDurationDHMS(mLastTimeSeconds, buffer, 32);
    mTimeLabel.setTextFormatted("UP %s", buffer);
}

void DebugScreen::onRenderMicros(){
    uint32_t t0 = Time::GetTickMicros() % 1000;
    uint32_t t1 = (Time::GetTickMicros() / 1000) % 1000;
    uint32_t t2 = (Time::GetTickMicros() / 1000000) % 1000;
    uint32_t t3 = (Time::GetTickMicros() / 1000000000);
    mMicrosLabel.setTextFormatted("CLOCK %03u'%03u'%03u'%03u", t3, t2, t1, t0);
}

void DebugScreen::onRenderHeapBlocks(){
    const uint32_t integer = mHeapInfo.largest_free_block/1000;
    const uint32_t fraction = mHeapInfo.largest_free_block%1000;
    mHeapBlocksLabel.setTextFormatted("BLK F%u A%u MAX %u.%03uk",
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
    mHeapBytesLabel.setTextFormatted("FREE %u.%03uk\nUSED %u.%03uk",
        int1, frac1,
        int2, frac2
    );
}

void DebugScreen::onRenderHeapMinimum(){
    const uint32_t int1 = mHeapInfo.minimum_free_bytes/1000;
    const uint32_t frac1 = mHeapInfo.minimum_free_bytes%1000;
    static char buffer[32];
    StringUtils::FormatDurationDHMS(mHeapMinimumTime, buffer, 32);
    mHeapMinimumLabel.setTextFormatted("MIN %u.%03uk\n%s",
        int1, frac1, buffer
    );
}

void DebugScreen::onRenderAppInfo(){
    mAppInfoLabel.setTextFormatted(
        "APP %s\nVIEW %s\nEVT %u  STATE %u",
        BehaviourIdToString(states().activeBehaviour.getBehaviour()), 
        ScreenIdToString(states().activeScreen.getScreen()),
        mLastEventCount, mLastStateChangesCount
    );
}

void DebugScreen::onRenderFermentationStatus(){
    mFermentationStatusLabel.setTextFormatted(
        "%s  SET %.1f C",
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
