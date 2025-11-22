#pragma once

#include "app/behaviours/generated/FermentationBehaviourAbs.h"
#include "app/engine/FermentationControlEngine.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {
public:
    
    FermentationBehaviour();
    
    void onLogicTick() final;

    void onHeartbeat(const Heartbeat& event) final;
    void onFanStatus(const FanStatus& event) final;
    void onFanSample(const FanSample& event) final;
    void onHeatpadStatus(const HeatpadStatus& event) final;
    void onTemperatureStatus(const TemperatureStatus& event) final;
    void onTemperatureSample(const TemperatureSample& event) final;
    void onButtonStateChanged(const ButtonStateChanged& event) final;
    void onButtonRepeat(const ButtonRepeat& event) final;
    void onEncoderStep(const EncoderStep& event) final;

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
