#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/behaviours/generated/FermentationBehaviourAbs.h"

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

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace Garbox