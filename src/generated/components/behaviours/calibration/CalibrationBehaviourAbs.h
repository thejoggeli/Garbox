#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
 

#include "core/application/behaviour/BehaviourAbs.h"
#include "generated/components/behaviours/calibration/CalibrationBehaviourStates.h"

namespace Garbox {

class Runtime;

class CalibrationBehaviourAbs : public BehaviourAbs {
public:

    // component constructor
    CalibrationBehaviourAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLogicTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeatEvent(const HeartbeatEvent& event) = 0;

    // state change handlers (to be implmeneted by user)
    virtual void onFanStatusStateChanged(const FanStatusState& state) = 0;
    virtual void onFanSampleStateChanged(const FanSampleState& state) = 0;

protected:

    // make typed events 
    FanCommandEvent makeFanCommandEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);

    CalibrationBehaviourStates& states();

private:

    std::optional<CalibrationBehaviourStates> mStates;

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox