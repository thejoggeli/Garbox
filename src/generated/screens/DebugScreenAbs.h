#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "core/lvgl/objects/LvObject.h"
#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"
#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/HeatpadProgressState.h"
#include "generated/states/types/DisplayStatusState.h"
#include "generated/states/types/DisplayDiagnosticsState.h"
#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"
#include "generated/states/types/ActiveBehaviourState.h"
#include "generated/states/types/ActiveScreenState.h"
#include "generated/states/types/FermentationStatusState.h"

#include "core/application/screen/ScreenAbs.h"

namespace Garbox {

class Runtime;

class DebugScreenAbs : public ScreenAbs {
public:

    // component constructor
    DebugScreenAbs();

    // state change handlers (to be implmeneted by user)
    virtual void onFanStatusStateChanged(const FanStatusState& state) {};
    virtual void onFanSampleStateChanged(const FanSampleState& state) {};
    virtual void onHeatpadStatusStateChanged(const HeatpadStatusState& state) {};
    virtual void onHeatpadSampleStateChanged(const HeatpadSampleState& state) {};
    virtual void onHeatpadProgressStateChanged(const HeatpadProgressState& state) {};
    virtual void onDisplayStatusStateChanged(const DisplayStatusState& state) {};
    virtual void onDisplayDiagnosticsStateChanged(const DisplayDiagnosticsState& state) {};
    virtual void onTemperatureStatusStateChanged(const TemperatureStatusState& state) {};
    virtual void onTemperatureSampleStateChanged(const TemperatureSampleState& state) {};
    virtual void onActiveBehaviourStateChanged(const ActiveBehaviourState& state) {};
    virtual void onActiveScreenStateChanged(const ActiveScreenState& state) {};
    virtual void onFermentationStatusStateChanged(const FermentationStatusState& state) {};

    // render functions (to be implmeneted by user)
    virtual void onRenderFanState() {};
    virtual void onRenderFanMeasuredRpm() {};
    virtual void onRenderHeatpadState() {};
    virtual void onRenderHeatpadDuty() {};
    virtual void onRenderHeatpadSense() {};
    virtual void onRenderHeatpadProgress() {};
    virtual void onRenderDisplayStatus() {};
    virtual void onRenderTemperatureState() {};
    virtual void onRenderTemperatureSample() {};
    virtual void onRenderTime() {};
    virtual void onRenderAppInfo() {};
    virtual void onRenderFermentationStatus() {};
    virtual void onRenderHeapBlocks() {};
    virtual void onRenderHeapBytes() {};
    virtual void onRenderHeapMinimum() {};

protected:

    // state access struct
    class States final {
    public:

        States(
            const FanStatusState& fanStatusState, // read
            const FanSampleState& fanSampleState, // read
            const HeatpadStatusState& heatpadStatusState, // read
            const HeatpadSampleState& heatpadSampleState, // read
            const HeatpadProgressState& heatpadProgressState, // read
            const DisplayStatusState& displayStatusState, // read
            const DisplayDiagnosticsState& displayDiagnosticsState, // read
            const TemperatureStatusState& temperatureStatusState, // read
            const TemperatureSampleState& temperatureSampleState, // read
            const ActiveBehaviourState& activeBehaviourState, // read
            const ActiveScreenState& activeScreenState, // read
            const FermentationStatusState& fermentationStatusState // read
        ):
            fanStatus(fanStatusState),
            fanSample(fanSampleState),
            heatpadStatus(heatpadStatusState),
            heatpadSample(heatpadSampleState),
            heatpadProgress(heatpadProgressState),
            displayStatus(displayStatusState),
            displayDiagnostics(displayDiagnosticsState),
            temperatureStatus(temperatureStatusState),
            temperatureSample(temperatureSampleState),
            activeBehaviour(activeBehaviourState),
            activeScreen(activeScreenState),
            fermentationStatus(fermentationStatusState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // readable states
        const FanStatusState& fanStatus;
        const FanSampleState& fanSample;
        const HeatpadStatusState& heatpadStatus;
        const HeatpadSampleState& heatpadSample;
        const HeatpadProgressState& heatpadProgress;
        const DisplayStatusState& displayStatus;
        const DisplayDiagnosticsState& displayDiagnostics;
        const TemperatureStatusState& temperatureStatus;
        const TemperatureSampleState& temperatureSample;
        const ActiveBehaviourState& activeBehaviour;
        const ActiveScreenState& activeScreen;
        const FermentationStatusState& fermentationStatus;
    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        const FanStatusState& fanStatus,
        const FanSampleState& fanSample,
        const HeatpadStatusState& heatpadStatus,
        const HeatpadSampleState& heatpadSample,
        const HeatpadProgressState& heatpadProgress,
        const DisplayStatusState& displayStatus,
        const DisplayDiagnosticsState& displayDiagnostics,
        const TemperatureStatusState& temperatureStatus,
        const TemperatureSampleState& temperatureSample,
        const ActiveBehaviourState& activeBehaviour,
        const ActiveScreenState& activeScreen,
        const FermentationStatusState& fermentationStatus
    );

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::sendEventToHost;

    friend class Runtime;

public:

    // init override from component
    void init(ComponentHostIfc& host) final;
    void updateScreen() final;
    void becomeEnabled() final;
    void becomeDisabled() final;

protected:

    LvObject mRoot;
    const uint32_t mScreenWidth = 0;
    const uint32_t mScreenHeight = 0;

    void setBackgroundColor(uint32_t color);

protected: 

    // render function indices 
    enum class RenderFn : uint8_t { 
        FanState = 0,
        FanMeasuredRpm,
        HeatpadState,
        HeatpadDuty,
        HeatpadSense,
        HeatpadProgress,
        DisplayStatus,
        TemperatureState,
        TemperatureSample,
        Time,
        AppInfo,
        FermentationStatus,
        HeapBlocks,
        HeapBytes,
        HeapMinimum,
        Count 
    }; 

    // render dispatcher (calls render methods)
    DirtyDispatcher mRenderDispatcher;
    uint32_t mDispatchedCount = 0;
    uint32_t getDispatchedCount() const { return mDispatchedCount; }

    // method to mark a render function dirty
    void markDirty(RenderFn fn);
    bool isMarkedDirty(RenderFn fn) const;

private: 

    // render trampolines
    static void renderFanStateTrampoline(void* context);
    static void renderFanMeasuredRpmTrampoline(void* context);
    static void renderHeatpadStateTrampoline(void* context);
    static void renderHeatpadDutyTrampoline(void* context);
    static void renderHeatpadSenseTrampoline(void* context);
    static void renderHeatpadProgressTrampoline(void* context);
    static void renderDisplayStatusTrampoline(void* context);
    static void renderTemperatureStateTrampoline(void* context);
    static void renderTemperatureSampleTrampoline(void* context);
    static void renderTimeTrampoline(void* context);
    static void renderAppInfoTrampoline(void* context);
    static void renderFermentationStatusTrampoline(void* context);
    static void renderHeapBlocksTrampoline(void* context);
    static void renderHeapBytesTrampoline(void* context);
    static void renderHeapMinimumTrampoline(void* context);

};

} // namespace Garbox