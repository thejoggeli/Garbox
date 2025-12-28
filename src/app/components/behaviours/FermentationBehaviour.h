#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/behaviours/fermentation/FermentationBehaviourAbs.h"
// ==== GENERATED END: include ====================================================

#include "app/engine/FermentationEngine.h"
#include "shared/types/EventAlias.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {

private:

    SoftwareTimer mStepTimer;

    HeatpadCommandPayload mLastHeatpadCommand; 
    FanCommandPayload mLastFanCommand;

    bool mFirstTick = true;

    FermentationEngine mFermentationEngine;

    bool checkTemperatureStatus();
    
    void sendFanCommand(bool enabled, float speed);
    void sendHeatpadCommand(bool enabled, float dutyCycle, uint32_t periodMicros);
    void updateFermentationStatus();

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    FermentationBehaviour();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    // generated tick handlers
    void onLogicTick() final;

    // generated event handlers
    void onHeartbeatEvent(const HeartbeatEvent& event) final;
    void onButtonEvent(const ButtonEvent& event) final;
    void onButtonRepeatEvent(const ButtonRepeatEvent& event) final;
    void onEncoderStepEvent(const EncoderStepEvent& event) final;
    void onRequestFermentationModeEvent(const RequestFermentationModeEvent& event) final;
    void onRequestTargetTemperatureEvent(const RequestTargetTemperatureEvent& event) final;
    void onRequestFanModeEvent(const RequestFanModeEvent& event) final;

    // generated state changed handlers
    void onFanSampleStateChanged(const FanSampleState& state) final;
    void onFanStatusStateChanged(const FanStatusState& state) final;
    void onTemperatureSampleStateChanged(const TemperatureSampleState& state) final;
    void onTemperatureStatusStateChanged(const TemperatureStatusState& state) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace
