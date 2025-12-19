#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"

#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class Runtime;

class CalibrationBehaviourAbs : public BehaviourAbs {
public:

    // component constructor
    CalibrationBehaviourAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLogicTick() {};

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeatEvent(const HeartbeatEvent& event) {};

    // state change handlers (to be implmeneted by user)
    virtual void onFanStatusStateChanged(const FanStatusState& state) {};
    virtual void onFanSampleStateChanged(const FanSampleState& state) {};

protected:

    // make typed events 
    FanCommandEvent makeFanCommandEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);

    // state access struct
    class States final {
    public:

        States(
            const FanStatusState& fanStatusState, // read
            const FanSampleState& fanSampleState // read
        ):
            fanStatus(fanStatusState),
            fanSample(fanSampleState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // readable states
        const FanStatusState& fanStatus;
        const FanSampleState& fanSample;
    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        const FanStatusState& fanStatus,
        const FanSampleState& fanSample
    );

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox