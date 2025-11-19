#pragma once 

#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class AppBehaviourAbs : public BehaviourAbs {
public:

    AppBehaviourAbs(ComponentId id);

    virtual void onLogicTick(){};

    virtual void onHeartbeat(const EventView<EventData::Heartbeat>& event){};
    virtual void onFanStatus(const EventView<EventData::FanStatus>& event){};
    virtual void onHeatpadStatus(const EventView<EventData::HeatpadStatus& event){};

};

} // namespace