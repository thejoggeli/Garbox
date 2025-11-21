#pragma once

#include "app/behaviours/generated/BaseBehaviourAbs.h"

namespace Garbox {

class FermentationBehaviour : public BaseBehaviourAbs {
public:

    FermentationBehaviour(const RuntimeContext& context);

    void onLogicTick() final;

    void onHeartbeat(const EventRead<EventPayload::Heartbeat>& event) final;
    void onFanStatus(const EventRead<EventPayload::FanStatus>& event) final;
    void onFanSample(const EventRead<EventPayload::FanSample>& event) final;
    void onHeatpadStatus(const EventRead<EventPayload::HeatpadStatus>& event) final;
    void onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event) final;
    void onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event) final;
    void onButton(const EventRead<EventPayload::Button>& event) final;
    void onButtonRepeat(const EventRead<EventPayload::ButtonRepeat>& event) final;
    void onEncoderStep(const EventRead<EventPayload::EncoderStep>& event) final;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace Garbox