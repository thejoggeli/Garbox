#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/screen/ScreenAbs.h"
#include "core/lvgl/objects/LvObject.h"
#include "generated/components/screens/debug/DebugScreenStates.h"

namespace Garbox {

class Runtime;

class DebugScreenAbs : public ScreenAbs {
public:

    // component constructor
    DebugScreenAbs();

    // state change handlers (to be implmeneted by user)
    virtual void onFanStatusStateChanged(const FanStatusState& state) = 0;
    virtual void onFanSampleStateChanged(const FanSampleState& state) = 0;
    virtual void onHeatpadStatusStateChanged(const HeatpadStatusState& state) = 0;
    virtual void onHeatpadSampleStateChanged(const HeatpadSampleState& state) = 0;
    virtual void onHeatpadProgressStateChanged(const HeatpadProgressState& state) = 0;
    virtual void onDisplayStatusStateChanged(const DisplayStatusState& state) = 0;
    virtual void onDisplayDiagnosticsStateChanged(const DisplayDiagnosticsState& state) = 0;
    virtual void onTemperatureStatusStateChanged(const TemperatureStatusState& state) = 0;
    virtual void onTemperatureSampleStateChanged(const TemperatureSampleState& state) = 0;
    virtual void onActiveBehaviourStateChanged(const ActiveBehaviourState& state) = 0;
    virtual void onActiveScreenStateChanged(const ActiveScreenState& state) = 0;
    virtual void onFermentationStatusStateChanged(const FermentationStatusState& state) = 0;

    // render functions (to be implmeneted by user)
    virtual void onRenderFanState() = 0;
    virtual void onRenderFanMeasuredRpm() = 0;
    virtual void onRenderHeatpadState() = 0;
    virtual void onRenderHeatpadDuty() = 0;
    virtual void onRenderHeatpadSense() = 0;
    virtual void onRenderHeatpadProgress() = 0;
    virtual void onRenderDisplayStatus() = 0;
    virtual void onRenderTemperatureState() = 0;
    virtual void onRenderTemperatureSample() = 0;
    virtual void onRenderTime() = 0;
    virtual void onRenderMicros() = 0;
    virtual void onRenderAppInfo() = 0;
    virtual void onRenderFermentationStatus() = 0;
    virtual void onRenderHeapBlocks() = 0;
    virtual void onRenderHeapBytes() = 0;
    virtual void onRenderHeapMinimum() = 0;

protected:

    DebugScreenStates& states();

private:

    std::optional<DebugScreenStates> mStates;

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::sendEventToHost;

    friend class Runtime;

public:

    // init override from component
    void init(ComponentHostIfc& host) final;
    void render() final;
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
        Micros,
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
    static void renderMicrosTrampoline(void* context);
    static void renderAppInfoTrampoline(void* context);
    static void renderFermentationStatusTrampoline(void* context);
    static void renderHeapBlocksTrampoline(void* context);
    static void renderHeapBytesTrampoline(void* context);
    static void renderHeapMinimumTrampoline(void* context);

};

} // namespace Garbox