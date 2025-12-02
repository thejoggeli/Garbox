#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/behaviours/abstract/FermentationBehaviourAbs.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {
public:

    FermentationBehaviour();

    // tick handlers
    void onLogicTick() final;

    // event handlers
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;
    virtual void onFanStatus(const FanStatusEvent& event) = 0;
    virtual void onFanSample(const FanSampleEvent& event) = 0;
    virtual void onHeatpadStatus(const HeatpadStatusEvent& event) = 0;
    virtual void onTemperatureStatus(const TemperatureStatusEvent& event) = 0;
    virtual void onTemperatureSample(const TemperatureSampleEvent& event) = 0;
    virtual void onButtonStateChanged(const ButtonStateChangedEvent& event) = 0;
    virtual void onButtonRepeat(const ButtonRepeatEvent& event) = 0;
    virtual void onEncoderStep(const EncoderStepEvent& event) = 0;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

};

} // namespace Garbox