// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviourAbs : public BehaviourAbs {
public:

    CalibrationBehaviourAbs();

    // tick handlers
    virtual void onLogicTick() = 0;

    // shorthand types for read events 
    using Heartbeat = EventRead<EventPayload::Heartbeat>;
    using FanStatus = EventRead<EventPayload::FanStatus>;
    using FanSample = EventRead<EventPayload::FanSample>;

    // receive event handlers
    virtual void onHeartbeat(const Heartbeat& event) = 0;
    virtual void onFanStatus(const FanStatus& event) = 0;
    virtual void onFanSample(const FanSample& event) = 0;

protected:

    // shorthand types for write events 
    using FanCommandWrite = EventWrite<EventPayload::FanCommand>;

    // make typed events
    FanCommandWrite makeFanCommandEvent();

    // send typed events
    void sendEvent(const FanCommandWrite& event);

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEvent;

};

} // namespace Garbox