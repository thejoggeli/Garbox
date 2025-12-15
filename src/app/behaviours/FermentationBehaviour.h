#pragma once

#include "app/behaviours/abstract/FermentationBehaviourAbs.h"
#include "app/engine/HeaterEngine.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {
public:

    FermentationBehaviour();
    
    void onLogicTick() final;

    void onHeartbeat(const HeartbeatEvent& event) final;
    void onButtonStateChanged(const ButtonStateChangedEvent& event) final;
    void onButtonRepeat(const ButtonRepeatEvent& event) final;
    void onEncoderStep(const EncoderStepEvent& event) final;

private:

    HeatpadCommandPayload mLastHeatpadCommand; 
    FanCommandPayload mLastFanCommand;

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

};

} // namespace
