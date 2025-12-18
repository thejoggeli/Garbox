#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class Runtime;

class FanControllerAbs : public ControllerAbs {
public:

    // component constructor
    FanControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() {};
    virtual void onOutputTick() {};

    // event handlers (to be implmeneted by user)
    virtual void onFanCommandEvent(const FanCommandEvent& event) {};

protected:

    // make typed events 
    FanStatusEvent makeFanStatusEvent();
    FanSampleEvent makeFanSampleEvent();

    // send typed events
    void sendEvent(const FanStatusEvent& event);
    void sendEvent(const FanSampleEvent& event);

    // state access struct
    class States final {
    public:

        States(
            FanStatusState& fanStatusState, // write
            FanSampleState& fanSampleState // write
        ):
            fanStatus(fanStatusState),
            fanSample(fanSampleState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // writable states
        FanStatusState& fanStatus;
        FanSampleState& fanSample;

    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        FanStatusState& fanStatus,
        FanSampleState& fanSample
    );

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox