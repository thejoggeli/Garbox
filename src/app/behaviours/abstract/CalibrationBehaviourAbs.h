#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/states/types/FanStatusState.h"
#include "app/states/types/FanSampleState.h"

#include "core/application/behaviour/BehaviourAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class GarboxRuntime;

class CalibrationBehaviourAbs : public BehaviourAbs {
public:

    // component constructor
    CalibrationBehaviourAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLogicTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeatEvent(const HeartbeatEvent& event) = 0;
    virtual void onFanStatusEvent(const FanStatusEvent& event) = 0;
    virtual void onFanSampleEvent(const FanSampleEvent& event) = 0;

    // state change handlers (to be implmeneted by user)
    virtual void onFanStatusStateChanged(const FanStatusState& state) = 0;
    virtual void onFanSampleStateChanged(const FanSampleState& state) = 0;

protected:

    // make typed events 
    FanCommandEvent makeFanCommandEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);

    // get readable states
    const FanStatusState& readFanStatusState();
    const FanSampleState& readFanSampleState();

private:

    // readable state pointers
    const FanStatusState* mFanStatusState = nullptr;
    const FanSampleState* mFanSampleState = nullptr;

    // dependency inject readable states
    void injectFanStatusState(const FanStatusState* state);
    void injectFanSampleState(const FanSampleState* state);

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox