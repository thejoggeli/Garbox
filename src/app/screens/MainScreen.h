#pragma once

#include "app/screens/abstract/MainScreenAbs.h"
#include "core/time/SoftwareTimer.h"
#include "core/util/helpers/DirtyDispatcher.h"

namespace Garbox {

class LvglObjects;

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

    void onUpdateScreen() final;

    void onFanStatus(const FanStatusEvent& event) final;
    void onFanSample(const FanSampleEvent& event) final;
    void onHeatpadStatus(const HeatpadStatusEvent& event) final;
    void onHeatpadSample(const HeatpadSampleEvent& event) final;
    void onTemperatureStatus(const TemperatureStatusEvent& event) final;
    void onTemperatureSample(const TemperatureSampleEvent& event) final;
    void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) final;
    void onFermentationStatus(const FermentationStatusEvent& event) final;
    void onDisplayStatus(const DisplayStatusEvent& event) final;

private:

    enum class Index : uint8_t {
        FanStatus = 0,
        FanMeasuredRpm,
        HeatpadState,
        HeatpadDuty,
        HeatpadProgress,
        HeatpadSense,
        DisplayStatus,
        ShtState,
        ShtSample,
        HeapSpace,
        AppState,
        FermentationStatus,
        Count,
    };

    struct ShadowState {
        FanState fanState;
        float fanTargetSpeed;
        float fanMeasuredRpm;
        HeatpadState heatpadState;
        float heatpadCurrentDuty;
        float heatpadNextDuty;
        uint32_t heatpadCurrentPeriod;
        uint32_t heatpadNextPeriod;
        uint32_t heatpadPwmProgressMicros;
        float heatpadMeasuredVoltage;
        float heatpadMeasuredCurrent;
        bool shtPower;
        bool shtDriver;
        bool shtReset;
        float shtTemp;
        float shtHum;
        uint32_t renderSkippedCount;
        float brightness;
        uint32_t heapSpace;
        uint32_t dirtyCount;
        BehaviourId behaviour;
        uint32_t eventCount;
        HeaterEngineState engineState;
        float engineTargetTemp;
        float engineMeasuredTemp;
        float engineMeasuredHum;
    };

    bool mFirstUpdate = true;

    LvglObjects& mObjects;
    DirtyDispatcher mDirtyDispatcher {static_cast<size_t>(Index::Count)};

    ShadowState mShadowState {};
    SoftwareTimer mHeapTimer;

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

    static void onUpdateFanState(void* context);
    static void onUpdateFanMeasuredRpm(void* context);
    static void onUpdateHeatpadState(void* context);
    static void onUpdateHeatpadDuty(void* context);
    static void onUpdateBoxPosition(void* context);
    static void onUpdateHeatpadSense(void* context);
    static void onUpdateDisplayStatus(void* context);
    static void onUpdateTemperatureState(void* context);
    static void onUpdateTemperatureSample(void* context);
    static void onUpdateHeapSpace(void* context);
    static void onUpdateAppInfo(void* context);
    static void onUpdateFermentationStatus(void* context);
};

} // namespace Garbox