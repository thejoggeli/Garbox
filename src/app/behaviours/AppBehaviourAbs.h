#pragma once 

#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class AppBehaviourAbs : public BehaviourAbs {
public:

    AppBehaviourAbs(ComponentId id, const RuntimeContext& context);

    virtual void onLogicTick(){};

    virtual void onHeartbeat(const EventRead<EventPayload::Heartbeat>& event) {};
    virtual void onFanStatus(const EventRead<EventPayload::FanStatus>& event) {};
    virtual void onHeatpadStatus(const EventRead<EventPayload::HeatpadStatus>& event) {};
    virtual void onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event) {};

};

} // namespace