#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviourAbs : public BehaviourAbs {
public:

    CalibrationBehaviourAbs();

    void receiveTick(TickPhase phase) final {};
    void receiveEvent(const EventHeader* event) final {};

    // tick handlers
    virtual void onLogicTick() = 0;

    // receive event handlers
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;
    virtual void onFanStatus(const FanStatusEvent& event) = 0;
    virtual void onFanSample(const FanSampleEvent& event) = 0;

protected:

    // make typed events
    FanCommandEvent makeFanCommandEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::publishEvent;

};

} // namespace Garbox