#pragma once

#include "app/behaviours/AppBehaviourAbs.h"
#include "app/engine/FermentationControlEngine.h"

namespace Garbox {

class FermentationBehaviour : public AppBehaviourAbs {
public:
    explicit FermentationBehaviour(ComponentId id, const RuntimeContext& context);
    
    void onLogicTick() final;
    virtual void onHeartbeat(const EventRead<EventPayload::Heartbeat>& event) final;
    virtual void onFanStatus(const EventRead<EventPayload::FanStatus>& event) final;
    virtual void onFanSample(const EventRead<EventPayload::FanSample>& event) final;
    virtual void onHeatpadStatus(const EventRead<EventPayload::HeatpadStatus>& event) final;
    virtual void onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event) final;
    virtual void onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event) final;

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
