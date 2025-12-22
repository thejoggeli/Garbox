#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "generated/guis/MainScreenGui.h"
#include "core/application/screen/ScreenAbs.h"
#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"
#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"
#include "generated/states/types/FermentationStatusState.h"

#include "core/application/screen/ScreenAbs.h"

namespace Garbox {

class Runtime;

class MainScreenAbs : public ScreenAbs {
public:

    // component constructor
    MainScreenAbs();

    // state change handlers (to be implmeneted by user)
    virtual void onFanStatusStateChanged(const FanStatusState& state) {};
    virtual void onFanSampleStateChanged(const FanSampleState& state) {};
    virtual void onHeatpadStatusStateChanged(const HeatpadStatusState& state) {};
    virtual void onHeatpadSampleStateChanged(const HeatpadSampleState& state) {};
    virtual void onTemperatureStatusStateChanged(const TemperatureStatusState& state) {};
    virtual void onTemperatureSampleStateChanged(const TemperatureSampleState& state) {};
    virtual void onFermentationStatusStateChanged(const FermentationStatusState& state) {};

    // render functions (to be implmeneted by user)
    virtual void onRenderFanInfo() {};
    virtual void onRenderPowerInfo() {};
    virtual void onRenderHumidityInfo() {};
    virtual void onRenderStatusInfo() {};
    virtual void onRenderMeasuredTemperatureSample() {};
    virtual void onRenderMeasuredTemperatureLabel() {};
    virtual void onRenderTargetTemperatureSample() {};
    virtual void onRenderHeatpadPowerSample() {};
    virtual void onRenderHeatpadPowerLabel() {};

protected:

    // state access struct
    class States final {
    public:

        States(
            const FanStatusState& fanStatusState, // read
            const FanSampleState& fanSampleState, // read
            const HeatpadStatusState& heatpadStatusState, // read
            const HeatpadSampleState& heatpadSampleState, // read
            const TemperatureStatusState& temperatureStatusState, // read
            const TemperatureSampleState& temperatureSampleState, // read
            const FermentationStatusState& fermentationStatusState // read
        ):
            fanStatus(fanStatusState),
            fanSample(fanSampleState),
            heatpadStatus(heatpadStatusState),
            heatpadSample(heatpadSampleState),
            temperatureStatus(temperatureStatusState),
            temperatureSample(temperatureSampleState),
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
        const TemperatureStatusState& temperatureStatus;
        const TemperatureSampleState& temperatureSample;
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
        const TemperatureStatusState& temperatureStatus,
        const TemperatureSampleState& temperatureSample,
        const FermentationStatusState& fermentationStatus
    );

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

private:

    MainScreenGui mGui;

protected:

    MainScreenGui::Objects& gui(); 
    const uint32_t mScreenWidth = 0;
    const uint32_t mScreenHeight = 0;

    void setBackgroundColor(uint32_t color);

protected: 

    // render function indices 
    enum class RenderFn : uint8_t { 
        FanInfo = 0,
        PowerInfo,
        HumidityInfo,
        StatusInfo,
        MeasuredTemperatureSample,
        MeasuredTemperatureLabel,
        TargetTemperatureSample,
        HeatpadPowerSample,
        HeatpadPowerLabel,
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
    static void renderFanInfoTrampoline(void* context);
    static void renderPowerInfoTrampoline(void* context);
    static void renderHumidityInfoTrampoline(void* context);
    static void renderStatusInfoTrampoline(void* context);
    static void renderMeasuredTemperatureSampleTrampoline(void* context);
    static void renderMeasuredTemperatureLabelTrampoline(void* context);
    static void renderTargetTemperatureSampleTrampoline(void* context);
    static void renderHeatpadPowerSampleTrampoline(void* context);
    static void renderHeatpadPowerLabelTrampoline(void* context);

};

} // namespace Garbox