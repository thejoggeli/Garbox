// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/behaviours/generated/FermentationBehaviourAbs.h"

namespace Garbox {

class FermentationBehaviour : public FermentationBehaviourAbs {
public:

    FermentationBehaviour(const RuntimeContext& context);

    void onLogicTick() final;

    void onHeartbeat(const EventRead<EventPayload::Heartbeat>& event) final;
    void onFanStatus(const EventRead<EventPayload::FanStatus>& event) final;
    void onFanSample(const EventRead<EventPayload::FanSample>& event) final;
    void onHeatpadStatus(const EventRead<EventPayload::HeatpadStatus>& event) final;
    void onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event) final;
    void onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event) final;
    void onButtonStateChanged(const EventRead<EventPayload::ButtonStateChanged>& event) final;
    void onButtonRepeat(const EventRead<EventPayload::ButtonRepeat>& event) final;
    void onEncoderStep(const EventRead<EventPayload::EncoderStep>& event) final;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace Garbox