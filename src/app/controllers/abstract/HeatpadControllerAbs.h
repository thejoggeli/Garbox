#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/states/types/HeatpadStatusState.h"
#include "app/states/types/HeatpadSampleState.h"

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class GarboxRuntime;

class HeatpadControllerAbs : public ControllerAbs {
public:

    // component constructor
    HeatpadControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() {};
    virtual void onOutputTick() {};

    // event handlers (to be implmeneted by user)
    virtual void onHeatpadCommandEvent(const HeatpadCommandEvent& event) {};

protected:

    // make typed events 
    HeatpadStatusEvent makeHeatpadStatusEvent();
    HeatpadSampleEvent makeHeatpadSampleEvent();

    // send typed events
    void sendEvent(const HeatpadStatusEvent& event);
    void sendEvent(const HeatpadSampleEvent& event);

    // state access struct
    class States final {
    public:

        States(
            HeatpadStatusState& heatpadStatusState, // write
            HeatpadSampleState& heatpadSampleState // write
        ):
            heatpadStatus(heatpadStatusState),
            heatpadSample(heatpadSampleState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // writable states
        HeatpadStatusState& heatpadStatus;
        HeatpadSampleState& heatpadSample;

    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        HeatpadStatusState& heatpadStatus,
        HeatpadSampleState& heatpadSample
    );

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox