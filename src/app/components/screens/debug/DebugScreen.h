#pragma once

#include <esp_heap_caps.h>
#include <esp_system.h>
#include "generated/screens/DebugScreenAbs.h"
#include "core/lvgl/objects/LvBox.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class DebugScreen : public DebugScreenAbs {
public:

    DebugScreen();

    // state change handlers (to be implmeneted by user)
    void onFanStatusStateChanged(const FanStatusState& state) final;
    void onFanSampleStateChanged(const FanSampleState& state) final;
    void onHeatpadStatusStateChanged(const HeatpadStatusState& state) final;
    void onHeatpadSampleStateChanged(const HeatpadSampleState& state) final;
    void onHeatpadProgressStateChanged(const HeatpadProgressState& state) final;
    void onDisplayStatusStateChanged(const DisplayStatusState& state) final;
    void onDisplayDiagnosticsStateChanged(const DisplayDiagnosticsState& state) final;
    void onTemperatureStatusStateChanged(const TemperatureStatusState& state) final;
    void onTemperatureSampleStateChanged(const TemperatureSampleState& state) final;
    void onActiveBehaviourStateChanged(const ActiveBehaviourState& state) final;
    void onActiveScreenStateChanged(const ActiveScreenState& state) final;
    void onFermentationStatusStateChanged(const FermentationStatusState& state) final;

    // render functions
    void onRenderFanState() final;
    void onRenderFanMeasuredRpm() final;
    void onRenderHeatpadState() final;
    void onRenderHeatpadDuty() final;
    void onRenderHeatpadSense() final;
    void onRenderHeatpadProgress() final;
    void onRenderDisplayStatus() final;
    void onRenderTemperatureState() final;
    void onRenderTemperatureSample() final;
    void onRenderTime() final;
    void onRenderAppInfo() final;
    void onRenderFermentationStatus() final;
    void onRenderHeapBlocks() final;
    void onRenderHeapBytes() final;
    void onRenderHeapMinimum() final;

private:

    uint32_t mLastTimeSeconds = 0;
    uint32_t mLastEventCount = 0;
    uint32_t mLastStateChangesCount = 0;

    multi_heap_info_t mHeapInfo;
    uint32_t mHeapMinimumTime = 0;

    LvBox mProgressBox;
    LvLabel mTimeLabel;
    LvLabel mFanStateLabel;
    LvLabel mFanMeasuredRpmLabel;
    LvLabel mHeatpadStateLabel;
    LvLabel mHeatpadDutyLabel;
    LvLabel mHeatpadSenseLabel;
    LvLabel mDisplayStatusLabel;
    LvLabel mTemperatureStateLabel;
    LvLabel mTemperatureSampleLabel;
    LvLabel mHeapBlocksLabel;
    LvLabel mHeapBytesLabel;
    LvLabel mHeapMinimumLabel;
    LvLabel mAppInfoLabel;
    LvLabel mFermentationStatusLabel;

    SoftwareTimer mHeapTimer;

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final;

    // helper to init a label with position and text
    void initLabel(LvLabel& label, int16_t x, int16_t y, const char* text);

};

} // namespace Garbox