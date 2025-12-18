#pragma once

#include "generated/behaviours/FermentationBehaviourAbs.h"
#include "app/engine/FermentationEngine.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {
public:

    FermentationBehaviour();
    
    void onLogicTick() final;

    void onHeartbeatEvent(const HeartbeatEvent& event) final;
    void onButtonStateChangedEvent(const ButtonStateChangedEvent& event) final;
    void onButtonRepeatEvent(const ButtonRepeatEvent& event) final;
    void onEncoderStepEvent(const EncoderStepEvent& event) final;

private:

    HeatpadCommandPayload mLastHeatpadCommand; 
    FanCommandPayload mLastFanCommand;
    FermentationStatusPayload mLastFermentationStatus;

    bool mFirstTick = true;
    bool mHeartbeatReceived = false;

    FermentationEngine mFermentationEngine;

    bool checkTemperatureStatus();

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
