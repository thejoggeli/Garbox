#pragma once

#include "app/behaviours/abstract/FermentationBehaviourAbs.h"
#include "app/engine/HeaterEngine.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {
public:

    FermentationBehaviour();
    
    void onLogicTick() final;

    void onHeartbeat(const HeartbeatEvent& event) final;
    void onFanStatus(const FanStatusEvent& event) final;
    void onFanSample(const FanSampleEvent& event) final;
    void onHeatpadStatus(const HeatpadStatusEvent& event) final;
    void onTemperatureStatus(const TemperatureStatusEvent& event) final;
    void onTemperatureSample(const TemperatureSampleEvent& event) final;
    void onButtonStateChanged(const ButtonStateChangedEvent& event) final;
    void onButtonRepeat(const ButtonRepeatEvent& event) final;
    void onEncoderStep(const EncoderStepEvent& event) final;

private:

    HeatpadCommandPayload mLastHeatpadCommand; 
    FanCommandPayload mLastFanCommand;
    FermentationStatusPayload mLastFermentationStatus;

    bool mFirstTick = true;
    bool mHeartbeatReceived = false;

    HeaterEngine mHeaterEngine;

    void doFanTestStep();
    
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    void sendFanCommand(bool enabled, float speed);
    void sendHeatpadCommand(bool enabled, float dutyCycle, uint32_t periodMicros);
    void sendFermentationStatus();

};

} // namespace
