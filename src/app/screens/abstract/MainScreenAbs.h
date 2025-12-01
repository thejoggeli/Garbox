#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class MainScreenAbs : public ScreenAbs {
protected:

    // updater callbacks to update specific parts of the screen (to be implmeneted by user)
    virtual void onApplyFanState() = 0;
    virtual void onApplyFanMeasuredRpm() = 0;
    virtual void onApplyHeatpadState() = 0;
    virtual void onApplyHeatpadDuty() = 0;
    virtual void onApplyBoxPosition() = 0;
    virtual void onApplyHeatpadSense() = 0;
    virtual void onApplyDisplayStatus() = 0;
    virtual void onApplyTemperatureState() = 0;
    virtual void onApplyTemperatureSample() = 0;
    virtual void onApplyHeapSpace() = 0;
    virtual void onApplyAppInfo() = 0;
    virtual void onApplyFermentationStatus() = 0;

public:

    MainScreenAbs();

    // receive events for updaters (called by runtime)
    void writeFanStatus(const FanStatusEvent& event);
    void writeFanSample(const FanSampleEvent& event);
    void writeHeatpadStatus(const HeatpadStatusEvent& event);
    void writeHeatpadSample(const HeatpadSampleEvent& event);
    void writeDisplayStatus(const DisplayStatusEvent& event);
    void writeTemperatureStatus(const TemperatureStatusEvent& event);
    void writeTemperatureSample(const TemperatureSampleEvent& event);
    void writeActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event);
    void writeFermentationStatus(const FermentationStatusEvent& event);

protected:

    enum class UpdaterIndex : uint8_t {
        FanState = 0,
        FanMeasuredRpm,
        HeatpadState,
        HeatpadDuty,
        BoxPosition,
        HeatpadSense,
        DisplayStatus,
        TemperatureState,
        TemperatureSample,
        HeapSpace,
        AppInfo,
        FermentationStatus,
        Count
    };

    // method to mark an updater index dirty (for manually updated values)
    void markDirty(UpdaterIndex index);
    bool isMarkedDirty(UpdaterIndex index) const;

    // shadow copy struct for 'FanState' updater
    struct FanStateShadowCopy {
        FanState state;
        float targetSpeed;
    } mFanState {};

    // shadow copy struct for 'FanMeasuredRpm' updater
    struct FanMeasuredRpmShadowCopy {
        float measuredRpm;
    } mFanMeasuredRpm {};

    // shadow copy struct for 'HeatpadState' updater
    struct HeatpadStateShadowCopy {
        HeatpadState state;
    } mHeatpadState {};

    // shadow copy struct for 'HeatpadDuty' updater
    struct HeatpadDutyShadowCopy {
        float currentDutyCycle;
        uint32_t currentPeriodMicros;
        float nextDutyCycle;
        uint32_t nextPeriodMicros;
    } mHeatpadDuty {};

    // shadow copy struct for 'BoxPosition' updater
    struct BoxPositionShadowCopy {
        float pwmProgressMicros;
    } mBoxPosition {};

    // shadow copy struct for 'HeatpadSense' updater
    struct HeatpadSenseShadowCopy {
        float measuredVoltage;
        float measuredCurrent;
    } mHeatpadSense {};

    // shadow copy struct for 'DisplayStatus' updater
    struct DisplayStatusShadowCopy {
        float brightness;
        uint32_t skipped;
    } mDisplayStatus {};

    // shadow copy struct for 'TemperatureState' updater
    struct TemperatureStateShadowCopy {
        bool driverEnabled;
        bool powerEnabled;
        bool resetting;
    } mTemperatureState {};

    // shadow copy struct for 'TemperatureSample' updater
    struct TemperatureSampleShadowCopy {
        float temperatureCelcius;
        float humidityRelative;
    } mTemperatureSample {};

    // shadow copy struct for 'HeapSpace' updater
    struct HeapSpaceShadowCopy {
        uint32_t heapSpace;
    } mHeapSpace {};

    // shadow copy struct for 'AppInfo' updater
    struct AppInfoShadowCopy {
        uint32_t eventCount;
        BehaviourId newBehaviour;
    } mAppInfo {};

    // shadow copy struct for 'FermentationStatus' updater
    struct FermentationStatusShadowCopy {
        HeaterEngineState heaterEngineState;
        float targetTemperature;
        float measuredTemperature;
        float measuredHumidity;
    } mFermentationStatus {};

    // write updater value methods
    void writeHeapSpaceHeapSpace(uint32_t heapSpace);
    void writeAppInfoEventCount(uint32_t eventCount);

    // make typed events 
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    void onInitScreen() final;

    // updater trampolines
    static void applyFanStateTrampoline(void* context);
    static void applyFanMeasuredRpmTrampoline(void* context);
    static void applyHeatpadStateTrampoline(void* context);
    static void applyHeatpadDutyTrampoline(void* context);
    static void applyBoxPositionTrampoline(void* context);
    static void applyHeatpadSenseTrampoline(void* context);
    static void applyDisplayStatusTrampoline(void* context);
    static void applyTemperatureStateTrampoline(void* context);
    static void applyTemperatureSampleTrampoline(void* context);
    static void applyHeapSpaceTrampoline(void* context);
    static void applyAppInfoTrampoline(void* context);
    static void applyFermentationStatusTrampoline(void* context);

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

    // hide dangerous raw access
    using ScreenAbs::registerUpdateHandler;
    using ScreenAbs::markDirtyRaw;
    using ScreenAbs::isMarkedDirtyRaw;

};

} // namespace Garbox