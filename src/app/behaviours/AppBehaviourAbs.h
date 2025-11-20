#pragma once 

#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class AppBehaviourAbs : public BehaviourAbs {
public:

    AppBehaviourAbs(ComponentId id);

    virtual void onLogicTick(){};

    virtual void onHeartbeat(const EventRead<EventData::Heartbeat>& event) {};
    virtual void onFanStatus(const EventRead<EventData::FanStatus>& event) {};
    virtual void onHeatpadStatus(const EventRead<EventData::HeatpadStatus>& event) {};
    virtual void onTemperatureStatus(const EventRead<EventData::TemperatureStatus>& event) {};

};

} // namespace