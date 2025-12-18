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
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onHeatpadCommandEvent(const HeatpadCommandEvent& event) = 0;

protected:

    // make typed events 
    HeatpadStatusEvent makeHeatpadStatusEvent();
    HeatpadSampleEvent makeHeatpadSampleEvent();

    // send typed events
    void sendEvent(const HeatpadStatusEvent& event);
    void sendEvent(const HeatpadSampleEvent& event);

    // state type accessors for getters and setters
    struct HeatpadStatusType {};
    struct HeatpadSampleType {};
    static constexpr HeatpadStatusType HeatpadStatus {};
    static constexpr HeatpadSampleType HeatpadSample {};

    // get writable states
    HeatpadStatusState& writeState(HeatpadStatusType type);
    HeatpadSampleState& writeState(HeatpadSampleType type);

private:

    // writable state pointers
    HeatpadStatusState* mHeatpadStatusState = nullptr;
    HeatpadSampleState* mHeatpadSampleState = nullptr;

    // dependency inject writable states
    void injectWritableState(HeatpadStatusState* state);
    void injectWritableState(HeatpadSampleState* state);

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox