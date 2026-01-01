#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/screen/ScreenAbs.h"
#include "generated/guis/MainScreenGui.h"
#include "generated/components/screens/main/MainScreenStates.h"

namespace Garbox {

class Runtime;

class MainScreenAbs : public ScreenAbs {
public:

    // component constructor
    MainScreenAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onUserInputTick() = 0;

    // render functions (to be implmeneted by user)
    virtual void onRenderFanInfo() = 0;
    virtual void onRenderPowerInfo() = 0;
    virtual void onRenderHumidityInfo() = 0;
    virtual void onRenderStatusInfo() = 0;
    virtual void onRenderMeasuredTemperatureLabel() = 0;
    virtual void onRenderHeatpadPowerLabel() = 0;
    virtual void onRenderTimeAxis() = 0;
    virtual void onRenderTimeSeries() = 0;
    virtual void onRenderMenu() = 0;

    // state change handlers (to be implmeneted by user)
    virtual void onFanStatusStateChanged(const FanStatusState& state) = 0;
    virtual void onFanSampleStateChanged(const FanSampleState& state) = 0;
    virtual void onHeatpadStatusStateChanged(const HeatpadStatusState& state) = 0;
    virtual void onHeatpadSampleStateChanged(const HeatpadSampleState& state) = 0;
    virtual void onTemperatureStatusStateChanged(const TemperatureStatusState& state) = 0;
    virtual void onTemperatureSampleStateChanged(const TemperatureSampleState& state) = 0;
    virtual void onFermentationStatusStateChanged(const FermentationStatusState& state) = 0;

protected:

    // make typed events 
    RequestFermentationModeEvent makeRequestFermentationModeEvent();
    RequestTargetTemperatureEvent makeRequestTargetTemperatureEvent();
    RequestFanModeEvent makeRequestFanModeEvent();

    // send typed events
    void sendEvent(const RequestFermentationModeEvent& event);
    void sendEvent(const RequestTargetTemperatureEvent& event);
    void sendEvent(const RequestFanModeEvent& event);

    MainScreenStates& states();

private:

    std::optional<MainScreenStates> mStates;

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
        MeasuredTemperatureLabel,
        HeatpadPowerLabel,
        TimeAxis,
        TimeSeries,
        Menu,
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
    static void renderMeasuredTemperatureLabelTrampoline(void* context);
    static void renderHeatpadPowerLabelTrampoline(void* context);
    static void renderTimeAxisTrampoline(void* context);
    static void renderTimeSeriesTrampoline(void* context);
    static void renderMenuTrampoline(void* context);

};

} // namespace Garbox