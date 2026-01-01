#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/screen/ScreenAbs.h"
#include "generated/guis/SimpleScreenGui.h"
#include "generated/components/screens/simple/SimpleScreenStates.h"

namespace Garbox {

class Runtime;

class SimpleScreenAbs : public ScreenAbs {
public:

    // component constructor
    SimpleScreenAbs();

    // render functions (to be implmeneted by user)
    virtual void onRenderEngineStatus() = 0;
    virtual void onRenderTargetTemperature() = 0;
    virtual void onRenderMeasuredTemperature() = 0;
    virtual void onRenderMeasuredHumidity() = 0;
    virtual void onRenderHeaterPower() = 0;
    virtual void onRenderFanStatus() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onButtonEvent(const ButtonEvent& event) = 0;
    virtual void onButtonRepeatEvent(const ButtonRepeatEvent& event) = 0;
    virtual void onEncoderStepEvent(const EncoderStepEvent& event) = 0;

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

    // send typed events
    void sendEvent(const RequestFermentationModeEvent& event);
    void sendEvent(const RequestTargetTemperatureEvent& event);

    SimpleScreenStates& states();

private:

    std::optional<SimpleScreenStates> mStates;

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

    SimpleScreenGui mGui;

protected:

    SimpleScreenGui::Objects& gui(); 
    const uint32_t mScreenWidth = 0;
    const uint32_t mScreenHeight = 0;

    void setBackgroundColor(uint32_t color);

protected: 

    // render function indices 
    enum class RenderFn : uint8_t { 
        EngineStatus = 0,
        TargetTemperature,
        MeasuredTemperature,
        MeasuredHumidity,
        HeaterPower,
        FanStatus,
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
    static void renderEngineStatusTrampoline(void* context);
    static void renderTargetTemperatureTrampoline(void* context);
    static void renderMeasuredTemperatureTrampoline(void* context);
    static void renderMeasuredHumidityTrampoline(void* context);
    static void renderHeaterPowerTrampoline(void* context);
    static void renderFanStatusTrampoline(void* context);

};

} // namespace Garbox