#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/HeatpadProgressState.h"

#include "core/application/controller/ControllerAbs.h"

namespace Garbox {

class Runtime;

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

    // state access struct
    class States final {
    public:

        States(
            HeatpadStatusState& heatpadStatusState, // write
            HeatpadSampleState& heatpadSampleState, // write
            HeatpadProgressState& heatpadProgressState // write
        ):
            heatpadStatus(heatpadStatusState),
            heatpadSample(heatpadSampleState),
            heatpadProgress(heatpadProgressState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // writable states
        HeatpadStatusState& heatpadStatus;
        HeatpadSampleState& heatpadSample;
        HeatpadProgressState& heatpadProgress;

    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        HeatpadStatusState& heatpadStatus,
        HeatpadSampleState& heatpadSample,
        HeatpadProgressState& heatpadProgress
    );

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox