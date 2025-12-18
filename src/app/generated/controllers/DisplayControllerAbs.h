#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app_generated/states/types/DisplayStatusState.h"
#include "app_generated/states/types/DisplayDiagnosticsState.h"

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class Runtime;

class DisplayControllerAbs : public ControllerAbs {
public:

    // component constructor
    DisplayControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onRenderTick() {};

    // event handlers (to be implmeneted by user)
    virtual void onDisplayCommandEvent(const DisplayCommandEvent& event) {};

protected:

    // make typed events 
    DisplayStatusEvent makeDisplayStatusEvent();

    // send typed events
    void sendEvent(const DisplayStatusEvent& event);

    // state access struct
    class States final {
    public:

        States(
            DisplayStatusState& displayStatusState, // write
            DisplayDiagnosticsState& displayDiagnosticsState // write
        ):
            displayStatus(displayStatusState),
            displayDiagnostics(displayDiagnosticsState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // writable states
        DisplayStatusState& displayStatus;
        DisplayDiagnosticsState& displayDiagnostics;

    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        DisplayStatusState& displayStatus,
        DisplayDiagnosticsState& displayDiagnostics
    );

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox