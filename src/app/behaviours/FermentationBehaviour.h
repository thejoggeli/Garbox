#pragma once

#include "app/behaviours/AppBehaviourAbs.h"
#include "app/engine/FermentationControlEngine.h"

namespace Garbox {

class FermentationBehaviour : public AppBehaviourAbs {
public:
    explicit FermentationBehaviour(ComponentId id = ComponentId::FermentationBehaviour);
    
    void onLogicTick() final;
    void onHeartbeat(const EventView<EventData::Heartbeat>& event) final;
    void onFanStatus(const EventView<EventData::FanStatus>& event) final;
    void onHeatpadStatus(const EventView<EventData::HeatpadStatus>& event) final;
    void onTemperatureStatus(const EventView<EventData::TemperatureStatus>& event) final;

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
