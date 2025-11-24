#pragma once

#include "app/behaviours/abstract/FermentationBehaviourAbs.h"
#include "app/engine/FermentationControlEngine.h"

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

    bool mHeartbeatReceived = false;
    bool mSwitchStateChanged = false;
    uint32_t mSwitchState = 0;

    FermentationControlEngine mControlEngine;

    void applySwitchState();
    void sendFanCommand(bool enabled, float speed);
    
    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace
