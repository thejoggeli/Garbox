#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/states/types/DisplayStatusState.h"
#include "app/states/types/DisplayDiagnosticsState.h"

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class GarboxRuntime;

class DisplayControllerAbs : public ControllerAbs {
public:

    // component constructor
    DisplayControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onRenderTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onDisplayCommandEvent(const DisplayCommandEvent& event) = 0;

protected:

    // make typed events 
    DisplayStatusEvent makeDisplayStatusEvent();

    // send typed events
    void sendEvent(const DisplayStatusEvent& event);

    // get writable states
    DisplayStatusState& stateDisplayStatus();
    DisplayDiagnosticsState& stateDisplayDiagnostics();

private:

    // writable state pointers
    DisplayStatusState* mDisplayStatusState = nullptr;
    DisplayDiagnosticsState* mDisplayDiagnosticsState = nullptr;

    // dependency inject writable states
    void injectDisplayStatusState(DisplayStatusState* state);
    void injectDisplayDiagnosticsState(DisplayDiagnosticsState* state);

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox