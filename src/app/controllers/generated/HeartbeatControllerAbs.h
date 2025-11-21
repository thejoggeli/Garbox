// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class HeartbeatControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    HeartbeatControllerAbs(const RuntimeContext& context);

    // tick handlers
    virtual void onHeartbeatTick() = 0;

protected:

    // make typed events
    EventWrite<EventPayload::Heartbeat> makeHeartbeatEvent();

    // send typed events
    void sendEvent(const EventWrite<EventPayload::Heartbeat>& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox