#pragma once
// Sections marked with GENERATED BEGIN/END may be overwritten by the generator.
// Do not modify or remove these sections. Changes outside these markers will be preserved.

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/screens/simple/SimpleScreenAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/lvgl/objects/LvArc.h"

namespace Garbox {

class SimpleScreen : public SimpleScreenAbs {
private:

    enum class ArcIndex : uint8_t {
        MeasuredTemperature,
        TargetTemperature,
    };

    void initArc(LvArc& arc);
    
    void setArcTemperature(ArcIndex index, float temperature);

public:
// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    SimpleScreen();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    // generated tick handlers
    void onUserInputTick() final;

    // generated render functions
    void onRender() final;
    void onRenderEngineStatus() final;
    void onRenderTargetTemperature() final;
    void onRenderMeasuredTemperature() final;
    void onRenderMeasuredHumidity() final;
    void onRenderHeaterPower() final;
    void onRenderFanStatus() final;

    // generated event handlers
    void onButtonEvent(const ButtonEvent& event) final;
    void onButtonRepeatEvent(const ButtonRepeatEvent& event) final;
    void onEncoderStepEvent(const EncoderStepEvent& event) final;

    // generated state changed handlers
    void onFanStatusStateChanged(const FanStatusState& state) final;
    void onFanSampleStateChanged(const FanSampleState& state) final;
    void onHeatpadStatusStateChanged(const HeatpadStatusState& state) final;
    void onHeatpadSampleStateChanged(const HeatpadSampleState& state) final;
    void onTemperatureStatusStateChanged(const TemperatureStatusState& state) final;
    void onTemperatureSampleStateChanged(const TemperatureSampleState& state) final;
    void onFermentationStatusStateChanged(const FermentationStatusState& state) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox