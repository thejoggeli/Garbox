// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/behaviours/generated/FermentationBehaviourAbs.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {
public:

    FermentationBehaviour(const RuntimeContext& context);

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

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace Garbox